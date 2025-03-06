#include "system/process.h"
#include <array>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "helpers/codehelpers.h"
#include "helpers/stringhelpers.h"
#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#endif

#define PROCESS_WAIT_TIMEOUT 50

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    Process::Process(const std::filesystem::path& path, const std::vector<std::string>& args, const std::filesystem::path& workingDir)
        : m_path{ path },
        m_args{ args },
        m_workingDirectory{ workingDir },
        m_running{ false },
        m_completed{ false },
        m_exitCode{ -1 },
#ifdef _WIN32
        m_read{ nullptr },
        m_write{ nullptr },
        m_pi{ 0 },
        m_job{ nullptr }
#else
        m_pid{ -1 }
#endif
    {
#ifdef _WIN32
        //Create console output pipes
        SECURITY_ATTRIBUTES sa{ 0 };
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = nullptr;
        if(!CreatePipe(&m_read, &m_write, &sa, 0))
        {
            throw std::runtime_error("Failed to create pipe.");
        }
        //Create job
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli{ 0 };
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        m_job = CreateJobObjectW(nullptr, nullptr);
        if(!m_job)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            CloseHandle(m_read);
            CloseHandle(m_write);
            throw std::runtime_error("Failed to create job object.");
        }
        SetInformationJobObject(m_job, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
        //Create process arguments
        std::wstring appArgs{ L"\"" + m_path.wstring() + L"\"" };
        for(const std::string& arg : m_args)
        {
            if(arg.find(' ') != std::string::npos && arg[0] != '\"')
            {
                appArgs += L" \"" + StringHelpers::wstr(arg) + L"\"";
            }
            else
            {
                appArgs += L" " + StringHelpers::wstr(arg);
            }
        }
        STARTUPINFOW si{ 0 };
        si.cb = sizeof(STARTUPINFOW);
        si.hStdError = m_write;
        si.hStdOutput = m_write;
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;
        //Create process
        if(!CreateProcessW(nullptr, appArgs.data(), nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, std::filesystem::is_directory(m_workingDirectory) && std::filesystem::exists(m_workingDirectory) ? m_workingDirectory.wstring().c_str() : nullptr, &si, &m_pi))
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            CloseHandle(m_read);
            CloseHandle(m_write);
            throw std::runtime_error("Failed to create process.");
        }
        AssignProcessToJobObject(m_job, m_pi.hProcess);
#else
        if(pipe(m_pipe) < 0)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            throw std::runtime_error("Failed to create pipe.");
        }
#endif
    }

    Process::~Process()
    {
        if(m_watchThread.joinable())
        {
            m_watchThread.join();
        }
#ifdef _WIN32
        CloseHandle(m_job);
        CloseHandle(m_read);
        CloseHandle(m_write);
        CloseHandle(m_pi.hProcess);
        CloseHandle(m_pi.hThread); 
#else
        close(m_pipe[0]);
        close(m_pipe[1]);
#endif
    }

    const std::filesystem::path& Process::getPath() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_path;
    }

    Event<ProcessExitedEventArgs>& Process::exited()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_exited;
    }

    bool Process::isRunning() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_running;
    }

    bool Process::hasCompleted() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_completed;
    }

    int Process::getExitCode() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_exitCode;
    }

    const std::string& Process::getOutput() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_output;
    }

    bool Process::start()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_running)
        {
            return true;
        }
        if(m_completed)
        {
            return false;
        }
#ifdef _WIN32
        if(!ResumeThread(m_pi.hThread))
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            return false;
        }
#else
        if((m_pid = fork()) < 0)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
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
            std::vector<char*> appArgs;
            appArgs.push_back(filename.data());
            for(std::string& arg : m_args)
            {
                appArgs.push_back(arg.data());
            }
            appArgs.push_back(nullptr);
            //Redirect console output
            close(m_pipe[0]);
            dup2(m_pipe[1], STDERR_FILENO);
            dup2(m_pipe[1], STDOUT_FILENO);
            close(m_pipe[1]);
            //Create process
            if(execvp(m_path.string().c_str(), appArgs.data()) < 0)
            {
                std::cerr << CodeHelpers::getLastSystemError() << std::endl;
                m_completed = true;
                m_exitCode = 1;
                exit(1);
            }
        }
        //Parent
        close(m_pipe[1]);
#endif
        m_watchThread = std::thread(&Process::watch, this);
        m_running = true;
        return true;
    }

    bool Process::kill()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_running)
        {
            return false;
        }
        //Kill child processes spawned by the process
#ifndef _WIN32
        ::kill(-m_pid, SIGTERM);
#endif
        //Kill process
#ifdef _WIN32
        CloseHandle(m_job);
        m_job = nullptr;
#else
        if(::kill(m_pid, SIGTERM) < 0)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            return false;
        }
#endif
        return true;
    }

    int Process::waitForExit()
    {
        while(!hasCompleted())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(PROCESS_WAIT_TIMEOUT));
        }
        return getExitCode();
    }

    void Process::watch()
    {
#ifndef _WIN32
        int status{ 0 };
#endif
        bool ended{ false };
        while(!ended)
        {
#ifdef _WIN32
            //Determine if ended
            ended = WaitForSingleObject(m_pi.hProcess, PROCESS_WAIT_TIMEOUT) == WAIT_OBJECT_0;
            //Read console output
            while(true)
            {
                std::array<char, 1024> buffer;
                DWORD bytes{ 0 };
                DWORD available{ 0 };
                if(!PeekNamedPipe(m_read, nullptr, 0, nullptr, &available, nullptr))
                {
                    break;
                }
                if(!available)
                {
                    break;
                }
                if(!ReadFile(m_read, buffer.data(), min(static_cast<unsigned int>(buffer.size()) - 1, available), &bytes, nullptr) || !bytes)
                {
                    break;
                }
                buffer[bytes] = 0;
                std::lock_guard<std::mutex> lock{ m_mutex };
                m_output += buffer.data();
            }
#else
            //Determine if ended
            while(waitpid(m_pid, &status, WNOHANG | WUNTRACED) > 0)
            {
                if(WIFEXITED(status) || WIFSIGNALED(status))
                {
                    ended = true;
                    close(m_pipe[0]);
                }
            }
            //Read console output
            char buffer[1024];
            ssize_t bytes{ 0 };
            while((bytes = read(m_pipe[0], buffer, sizeof(buffer) - 1)) > 0)
            {
                buffer[bytes] = '\0';
                std::lock_guard<std::mutex> lock{ m_mutex };
                m_output += buffer;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(PROCESS_WAIT_TIMEOUT));
#endif
        }
        std::unique_lock<std::mutex> lock{ m_mutex };
#ifdef _WIN32
        if(!m_job)
        {
            m_exitCode = -1;
        }
        else
        {
            DWORD exitCode{ 0 };
            if(GetExitCodeProcess(m_pi.hProcess, &exitCode))
            {
                m_exitCode = static_cast<int>(exitCode);
            }
            else
            {
                m_exitCode = -1;
            }
        }
#else
        m_exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
#endif
        m_running = false;
        m_completed = true;
        lock.unlock();
        m_exited.invoke({ m_exitCode, m_output });
    }
}
