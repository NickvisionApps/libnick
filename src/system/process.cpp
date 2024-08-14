#include "system/process.h"
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "filesystem/userdirectories.h"
#include "helpers/codehelpers.h"
#include "helpers/stringhelpers.h"
#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#endif

using namespace Nickvision::Filesystem;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    Process::Process(const std::filesystem::path& path, const std::vector<std::string>& args)
        : m_path{ path },
        m_args{ args },
        m_running{ false },
        m_completed{ false },
        m_exitCode{ -1 },
#ifdef _WIN32
        m_read{ nullptr },
        m_write{ nullptr },
        m_pi{ 0 }
#else
        m_pid{ -1 },
        m_consoleFilePath{ UserDirectories::get(UserDirectory::Cache) / (StringHelpers::newUuid() + ".lnproc") }
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
        if(!CreateProcessW(nullptr, appArgs.data(), nullptr, nullptr, TRUE, CREATE_SUSPENDED | CREATE_NEW_PROCESS_GROUP, nullptr, nullptr, &si, &m_pi))
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            CloseHandle(m_read);
            CloseHandle(m_write);
            throw std::runtime_error("Failed to create process.");
        }
#else
        //Fork
        int fd{ open(m_consoleFilePath.string().c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) };
        if(fd < 0)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            throw std::runtime_error("Failed to create file.");
        }
        if((m_pid = fork()) < 0)
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            throw std::runtime_error("Failed to create fork.");
        }
        else if(m_pid == 0) //child
        {
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
            dup2(fd, STDERR_FILENO);
            dup2(fd, STDOUT_FILENO);
            //Create process
            raise(SIGSTOP);
            if(execvp(m_path.string().c_str(), appArgs.data()) < 0)
            {
                std::cerr << CodeHelpers::getLastSystemError() << std::endl;
                m_completed = true;
                m_exitCode = 1;
                exit(1);
            }
        }
        close(fd);
#endif
    }

    Process::~Process()
    {
        if(m_watchThread.joinable())
        {
            m_watchThread.join();
        }
#ifdef _WIN32
        CloseHandle(m_read);
        CloseHandle(m_write);
        CloseHandle(m_pi.hProcess);
        CloseHandle(m_pi.hThread);
#else
        std::filesystem::remove(m_consoleFilePath);
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
#else
        waitpid(m_pid, nullptr, WUNTRACED);
        if(::kill(m_pid, SIGCONT) < 0)
#endif
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            return false;
        }
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
#ifdef _WIN32
        if(!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, m_pi.dwProcessId))
#else
        if(::kill(-m_pid, SIGTERM) < 0)
#endif
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            return false;
        }
        //Kill process
#ifdef _WIN32
        if(!TerminateProcess(m_pi.hProcess, 0))
#else
        if(::kill(m_pid, SIGTERM) < 0)
#endif
        {
            std::cerr << CodeHelpers::getLastSystemError() << std::endl;
            return false;
        }
        m_exitCode = -1;
        m_running = false;
        m_completed = true;
        return true;
    }

    int Process::waitForExit()
    {
        if(hasCompleted())
        {
            return m_exitCode;
        }
        while(!hasCompleted())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return getExitCode();
    }

    void Process::watch()
    {
#ifdef __linux__
        int status{ 0 };
#endif
        bool ended{ false };
        while(!ended)
        {
#ifdef _WIN32
            //Determine if ended
            ended = WaitForSingleObject(m_pi.hProcess, 50) == WAIT_OBJECT_0;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            while(waitpid(m_pid, &status, WNOHANG | WUNTRACED | WCONTINUED) > 0)
            {
                if(WIFEXITED(status) || WIFSIGNALED(status))
                {
                    ended = true;
                }
            }
            //Read console output
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::ifstream file{ m_consoleFilePath };
            std::stringstream buffer;
            buffer << file.rdbuf();
            m_output = buffer.str();
#endif
        }
#ifdef _WIN32
        DWORD exitCode{ 0 };
        GetExitCodeProcess(m_pi.hProcess, &exitCode);
#else
        int exitCode{ WIFEXITED(status) ? WEXITSTATUS(status) : -1 };
#endif
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_exitCode = static_cast<int>(exitCode);
        m_running = false;
        m_completed = true;
        lock.unlock();
        m_exited.invoke({ m_exitCode, m_output });
    }
}