#include "system/process.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include "helpers/sizehelpers.h"
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <psapi.h>
#include <tlhelp32.h>
#else
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#endif
#ifdef __APPLE__
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <AvailabilityMacros.h>
#endif

#define PROCESS_WAIT_TIMEOUT 50

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
#ifdef _WIN32
    static std::vector<DWORD> getJobProcesses(HANDLE job) noexcept
    {
        std::vector<DWORD> pids;
        size_t bufferSize{ sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + 255 * sizeof(ULONG_PTR) };
        JOBOBJECT_BASIC_PROCESS_ID_LIST* buffer{ reinterpret_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST*>(malloc(bufferSize)) };
        if(QueryInformationJobObject(job, JobObjectBasicProcessIdList, buffer, static_cast<DWORD>(bufferSize), nullptr))
        {
            for(DWORD i{ 0 }; i < buffer->NumberOfProcessIdsInList; i++)
            {
                pids.push_back(static_cast<DWORD>(buffer->ProcessIdList[i]));
            }
        }
        free(buffer);
        return pids;
    }
#endif

    Process::Process(const std::filesystem::path& path, const std::vector<std::string>& args, const std::filesystem::path& workingDir)
        : m_path{ path },
        m_args{ args },
        m_workingDirectory{ workingDir },
        m_state{ ProcessState::Created },
        m_exitCode{ -1 },
#ifdef _WIN32
        m_childOutRead{ nullptr },
        m_childOutWrite{ nullptr },
        m_childInRead{ nullptr },
        m_childInWrite{ nullptr },
        m_pi{},
        m_job{ nullptr },
        m_lastProcKernelTime{ 0 },
        m_lastProcUserTime{ 0 },
        m_lastSysKernelTime{ 0 },
        m_lastSysUserTime{ 0 }
#else
        m_pid{ -1 },
        m_lastUserTime{ 0 },
        m_lastSystemTime{ 0 }
#endif
    {
#ifdef _WIN32
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = nullptr;
        //Create console output pipes
        if(!CreatePipe(&m_childOutRead, &m_childOutWrite, &sa, 0))
        {
            throw std::runtime_error("Failed to create output pipes.");
        }
        //Create console input pipes
        if(!CreatePipe(&m_childInRead, &m_childInWrite, &sa, 0))
        {
            throw std::runtime_error("Failed to create input pipes.");
        }
        //Create job
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli{};
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
        m_job = CreateJobObjectW(&sa, nullptr);
        if(!m_job)
        {
            CloseHandle(m_childOutRead);
            CloseHandle(m_childOutWrite);
            CloseHandle(m_childInRead);
            CloseHandle(m_childInWrite);
            throw std::runtime_error("Failed to create job object.");
        }
        SetInformationJobObject(m_job, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
        //Create process arguments
        std::wstring appArgs{ StringHelpers::wstr(StringHelpers::quote(m_path.string())) };
        for(const std::string& arg : m_args)
        {
            if(arg.find(' ') != std::string::npos)
            {
                appArgs += L" " + StringHelpers::wstr(StringHelpers::quote(arg));
            }
            else
            {
                appArgs += L" " + StringHelpers::wstr(arg);
            }
        }
        STARTUPINFOW si{};
        si.cb = sizeof(STARTUPINFOW);
        si.hStdError = m_childOutWrite;
        si.hStdOutput = m_childOutWrite;
        si.hStdInput = m_childInRead;
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;
        //Create process
        if(!CreateProcessW(nullptr, appArgs.data(), nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, std::filesystem::exists(m_workingDirectory) && std::filesystem::is_directory(m_workingDirectory) ? m_workingDirectory.wstring().c_str() : nullptr, &si, &m_pi))
        {
            CloseHandle(m_job);
            CloseHandle(m_childOutRead);
            CloseHandle(m_childOutWrite);
            CloseHandle(m_childInRead);
            CloseHandle(m_childInWrite);
            throw std::runtime_error("Failed to create process.");
        }
        CloseHandle(m_childOutWrite);
        CloseHandle(m_childInRead);
        AssignProcessToJobObject(m_job, m_pi.hProcess);
#else
        if(pipe(m_childOutPipes) < 0)
        {
            throw std::runtime_error("Failed to create output pipes.");
        }
        if(pipe(m_childInPipes) < 0)
        {
            throw std::runtime_error("Failed to create input pipes.");
        }
#endif
    }

    Process::~Process() noexcept
    {
        if(m_watchThread.joinable())
        {
            m_watchThread.join();
        }
#ifdef _WIN32
        CloseHandle(m_job);
        CloseHandle(m_childOutRead);
        CloseHandle(m_childInWrite);
        CloseHandle(m_pi.hProcess);
        CloseHandle(m_pi.hThread); 
#else
        close(m_childOutPipes[0]);
        close(m_childOutPipes[1]);
        close(m_childInPipes[0]);
        close(m_childInPipes[1]);
#endif
    }

    Event<ProcessExitedEventArgs>& Process::exited() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_exited;
    }

    const std::filesystem::path& Process::getPath() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_path;
    }

    ProcessState Process::getState() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_state;
    }

    int Process::getExitCode() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_exitCode;
    }

    const std::string& Process::getOutput() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_output;
    }

    double Process::getCPUUsage() const noexcept
    {
        if(m_state != ProcessState::Running)
        {
            return 0.0;
        }
#ifdef _WIN32
        FILETIME creationTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        if(GetProcessTimes(m_pi.hProcess, &creationTime, &exitTime, &kernelTime, &userTime))
        {
            FILETIME sysIdleTime;
            FILETIME sysKernelTime;
            FILETIME sysUserTime;
            if(GetSystemTimes(&sysIdleTime, &sysKernelTime, &sysUserTime))
            {
                unsigned long long sysKernel{ (static_cast<unsigned long long>(sysKernelTime.dwHighDateTime) << 32) | sysKernelTime.dwLowDateTime };
                unsigned long long sysUser{ (static_cast<unsigned long long>(sysUserTime.dwHighDateTime) << 32) | sysUserTime.dwLowDateTime };
                unsigned long long procKernel{ (static_cast<unsigned long long>(kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime };
                unsigned long long procUser{ (static_cast<unsigned long long>(userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime };
                unsigned long long sysDelta{ (sysKernel - m_lastSysKernelTime) + (sysUser - m_lastSysUserTime) };
                unsigned long long procDelta{ (procKernel - m_lastProcKernelTime) + (procUser - m_lastProcUserTime) };
                m_lastSysKernelTime = sysKernel;
                m_lastSysUserTime = sysUser;
                m_lastProcKernelTime = procKernel;
                m_lastProcUserTime = procUser;
                if(sysDelta != 0)
                {
                    return (static_cast<double>(procDelta) / sysDelta) * 100.0;
                }
            }
        }
#elif defined(__linux__)
        std::ifstream proc{ "/proc/" + std::to_string(m_pid) + "/stat" };
        std::ifstream stat{ "/proc/stat" };
        if(proc.is_open() && stat.is_open())
        {
            std::string line;
            //Proc information
            std::getline(proc, line);
            std::vector<std::string> procTokens{ StringHelpers::split(line, ' ', false) };
            unsigned long long userTime{ std::stoull(procTokens[13]) + std::stoull(procTokens[14]) };
            //Sys information
            std::getline(stat, line);
            std::vector<std::string> statTokens{ StringHelpers::split(line, ' ', false) };
            unsigned long long systemTime{ 0 };
            for(int i = 1; i < 9; i++)
            {
                systemTime += std::stoull(statTokens[i]);
            }
            //Get usage
            unsigned long long sysDelta{ systemTime - m_lastSystemTime };
            unsigned long long procDelta{ userTime - m_lastUserTime };
            m_lastSystemTime = systemTime;
            m_lastUserTime = userTime;
            if(sysDelta > 0)
            {
                return (procDelta * 100) / sysDelta;
            }
        }
#elif defined(__APPLE__)
        task_t task;
        if(task_for_pid(mach_task_self(), m_pid, &task) == KERN_SUCCESS)
        {
            task_basic_info_data_t info;
            mach_msg_type_number_t count{ TASK_BASIC_INFO_COUNT };
            if(task_info(task, TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS)
            {
                unsigned long long sysDelta{ info.system_time.seconds - m_lastSystemTime };
                unsigned long long procDelta{ info.user_time.seconds - m_lastUserTime };
                m_lastSystemTime = info.system_time.seconds;
                m_lastUserTime = info.user_time.seconds;
                if(sysDelta > 0)
                {
                    return (procDelta * 100) / sysDelta;
                }
            }
        }
#endif
        return 0.0;
    }

    unsigned long long Process::getRAMUsage() const noexcept
    {
        if(m_state != ProcessState::Running)
        {
            return 0L;
        }
#ifdef _WIN32
        PROCESS_MEMORY_COUNTERS pmc{};
        unsigned long long mem{ 0 };
        HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        if(snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 pe{};
            pe.dwSize = sizeof(PROCESSENTRY32);
            if(Process32First(snapshot, &pe))
            {
                do
                {
                    if(pe.th32ParentProcessID == m_pi.dwProcessId || pe.th32ProcessID == m_pi.dwProcessId)
                    {
                        HANDLE process{ OpenProcess(PROCESS_QUERY_INFORMATION, false, pe.th32ProcessID) };
                        if(!process)
                        {
                            continue;
                        }
                        if(K32GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
                        {
                            mem += pmc.WorkingSetSize;
                        }
                        CloseHandle(process);
                    }
                } while(Process32Next(snapshot, &pe));
            }
            CloseHandle(snapshot);
        }
        return mem;
#elif defined(__linux__)
        std::ifstream proc{ "/proc/" + std::to_string(m_pid) + "/status" };
        if(proc.is_open())
        {
            std::string line;
            while(std::getline(proc, line))
            {
                if(line.find("VmRSS:") != std::string::npos)
                {
                    std::vector<std::string> fields{ StringHelpers::split(line, ' ', false) };
                    return SizeHelpers::kilobytesToBytes(std::stoull(fields[1]));
                }
            }
        }
#elif defined(__APPLE__)
        task_t task;
        if(task_for_pid(mach_task_self(), m_pid, &task) == KERN_SUCCESS)
        {
    /* Code for modern macOS */
    #if MAC_OS_X_VERSION_MIN_REQUIRED >= 1090
            task_vm_info_data_t info;
            mach_msg_type_number_t count{ sizeof(task_vm_info_data_t) };
            if(task_info(task, TASK_VM_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS)
            {
                return info.phys_footprint;
            }
    /* Fallback for macOS < 10.9 */
    #else
            task_basic_info_data_t info;
            mach_msg_type_number_t count{ TASK_BASIC_INFO_COUNT };
            if(task_info(task, TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) == KERN_SUCCESS)
            {
                return info.resident_size;
            }
    #endif
        }
#endif
        return 0L;
    }

    bool Process::start() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_state == ProcessState::Running)
        {
            return true;
        }
        else if(m_state != ProcessState::Created)
        {
            return false;
        }
#ifdef _WIN32
        if(ResumeThread(m_pi.hThread) == static_cast<DWORD>(-1))
        {
            return false;
        }
#else
        if((m_pid = fork()) < 0)
        {
            return false;
        }
        //Child
        else if(m_pid == 0)
        {
            //Change working directory
            if(std::filesystem::is_directory(m_workingDirectory) && std::filesystem::exists(m_workingDirectory))
            {
                chdir(m_workingDirectory.string().c_str());
            }
            //Create process arguments
            std::string filename{ m_path.filename().string() };
            if(filename.find(' ') != std::string::npos)
            {
                filename = StringHelpers::quote(filename);
            }
            std::vector<char*> appArgs;
            appArgs.push_back(filename.data());
            for(std::string& arg : m_args)
            {
                appArgs.push_back(arg.data());
            }
            appArgs.push_back(nullptr);
            //Redirect console output
            close(m_childOutPipes[0]);
            close(m_childInPipes[1]);
            dup2(m_childOutPipes[1], STDERR_FILENO);
            dup2(m_childOutPipes[1], STDOUT_FILENO);
            dup2(m_childInPipes[0], STDIN_FILENO);
            close(m_childOutPipes[1]);
            close(m_childInPipes[0]);
            //Create process
            if(execvp(m_path.string().c_str(), appArgs.data()) < 0)
            {
                m_state = ProcessState::Completed;
                m_exitCode = 1;
                exit(1);
            }
        }
        //Parent
        close(m_childOutPipes[1]);
        close(m_childInPipes[0]);
#endif
        m_watchThread = std::thread(&Process::watch, this);
        m_state = ProcessState::Running;
        return true;
    }

    bool Process::kill() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_state != ProcessState::Running && m_state != ProcessState::Paused)
        {
            return false;
        }
#ifndef _WIN32
        ::kill(-m_pid, SIGTERM);
        if(::kill(m_pid, SIGTERM) < 0)
#else
        if(!TerminateJobObject(m_job, -1))
#endif
        {
            return false;
        }
        m_state = ProcessState::Killed;
        return true;
    }

    bool Process::resume() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_state != ProcessState::Paused)
        {
            return false;
        }
#ifndef _WIN32
        ::kill(-m_pid, SIGCONT);
        if(::kill(m_pid, SIGCONT) < 0)
        {
            return false;
        }
#else
        std::vector<DWORD> pids{ getJobProcesses(m_job) };
        for(DWORD pid : pids)
        {
            HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0) };
            if(snapshot != INVALID_HANDLE_VALUE)
            {
                THREADENTRY32 entry{};
                entry.dwSize = sizeof(THREADENTRY32);
                if(Thread32First(snapshot, &entry))
                {
                    do
                    {
                        if(entry.th32OwnerProcessID == pid)
                        {
                            HANDLE thread{ OpenThread(THREAD_SUSPEND_RESUME, false, entry.th32ThreadID) };
                            if(thread)
                            {
                                ResumeThread(thread);
                                CloseHandle(thread);
                            }
                        }
                    } while(Thread32Next(snapshot, &entry));
                }
                CloseHandle(snapshot);
            }
        }
#endif
        m_state = ProcessState::Running;
        return true;
    }

    bool Process::pause() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_state != ProcessState::Running)
        {
            return false;
        }
#ifndef _WIN32
        ::kill(-m_pid, SIGSTOP);
        if(::kill(m_pid, SIGSTOP) < 0)
        {
            return false;
        }
#else
        std::vector<DWORD> pids{ getJobProcesses(m_job) };
        for(DWORD pid : pids)
        {
            HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0) };
            if(snapshot != INVALID_HANDLE_VALUE)
            {
                THREADENTRY32 entry{};
                entry.dwSize = sizeof(THREADENTRY32);
                if(Thread32First(snapshot, &entry))
                {
                    do
                    {
                        if(entry.th32OwnerProcessID == pid)
                        {
                            HANDLE thread{ OpenThread(THREAD_SUSPEND_RESUME, false, entry.th32ThreadID) };
                            if(thread)
                            {
                                SuspendThread(thread);
                                CloseHandle(thread);
                            }
                        }
                    } while(Thread32Next(snapshot, &entry));
                }
                CloseHandle(snapshot);
            }
        }
#endif
        m_state = ProcessState::Paused;
        return true;
    }

    int Process::waitForExit() noexcept
    {
        while(getState() != ProcessState::Completed)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(PROCESS_WAIT_TIMEOUT));
        }
        return getExitCode();
    }

    bool Process::send(const std::string& s) noexcept
    {
        if(m_state != ProcessState::Running)
        {
            return false;
        }
#ifndef _WIN32
        return write(m_childInPipes[1], s.data(), s.size()) == s.size();
#else
        return WriteFile(m_childInWrite, s.data(), static_cast<DWORD>(s.size()), nullptr, nullptr);
#endif
    }

    bool Process::sendCommand(std::string s) noexcept
    {
#ifndef _WIN32
        s += "\n";
#else
        s += "\r\n";
#endif
        return send(s);
    }

    void Process::watch() noexcept
    {
#ifdef _WIN32
        DWORD exitCode{ 0 };
        do
        {
            //Determine if ended
            if(!GetExitCodeProcess(m_pi.hProcess, &exitCode))
            {
                exitCode = STILL_ACTIVE;
            }
            //Read console output
            while(true)
            {
                DWORD available{ 0 };
                if(!PeekNamedPipe(m_childOutRead, nullptr, 0, nullptr, &available, nullptr) || available == 0)
                {
                    break;
                }
                std::vector<char> buffer(available);
                DWORD read{ 0 };
                if(!ReadFile(m_childOutRead, buffer.data(), static_cast<DWORD>(buffer.size()), &read, nullptr) || read == 0)
                {
                    break;
                }
                std::lock_guard<std::mutex> lock{ m_mutex };
                m_output += std::string(buffer.data(), buffer.data() + read);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(PROCESS_WAIT_TIMEOUT));
        } while(exitCode == STILL_ACTIVE);
#else
        int status{ 0 };
        bool ended{ false };
        while(!ended)
        {
            //Determine if ended
            while(waitpid(m_pid, &status, WNOHANG | WUNTRACED) > 0)
            {
                if(WIFEXITED(status) || WIFSIGNALED(status))
                {
                    ended = true;
                    close(m_childOutPipes[0]);
                }
            }
            //Read console output
            char buffer[1024];
            ssize_t bytes{ 0 };
            while((bytes = read(m_childOutPipes[0], buffer, sizeof(buffer) - 1)) > 0)
            {
                std::lock_guard<std::mutex> lock{ m_mutex };
                m_output += std::string(buffer, buffer + bytes);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(PROCESS_WAIT_TIMEOUT));
        }
#endif
        std::unique_lock<std::mutex> lock{ m_mutex };
#ifdef _WIN32
        m_exitCode = static_cast<int>(exitCode);
#else
        m_exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
        m_state = ProcessState::Completed;
        lock.unlock();
        m_exited.invoke({ m_exitCode, m_output });
    }
}
