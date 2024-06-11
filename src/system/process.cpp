#include "system/process.h"
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "filesystem/userdirectories.h"
#include "helpers/stringhelpers.h"
#ifdef __linux__
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#endif

using namespace Nickvision::Filesystem;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;

namespace Nickvision::System
{
    static std::string getLastErrorAsString()
    {
#ifdef _WIN32
        DWORD errorMessageID{ GetLastError() };
        if(errorMessageID == 0) 
        {
            return std::string();
        }
        LPSTR messageBuffer{ nullptr };
        size_t size{ FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr) };
        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        return message;
#elif defined(__linux__)
        return std::string(strerror(errno));
#endif
    }

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
#elif defined(__linux__)
        m_pid{ -1 },
        m_consoleFilePath{ UserDirectories::getCache() / (StringHelpers::newGuid() + ".lnproc") }
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
            appArgs += L" " + StringHelpers::wstr(arg);
        }
        STARTUPINFOW si{ 0 };
        si.cb = sizeof(STARTUPINFOW);
        si.hStdError = m_write;
        si.hStdOutput = m_write;
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        si.wShowWindow = SW_HIDE;
        //Create process
        if(!CreateProcessW(nullptr, LPWSTR(appArgs.c_str()), nullptr, nullptr, TRUE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &m_pi))
        {
            std::cerr << getLastErrorAsString() << std::endl;
            CloseHandle(m_read);
            CloseHandle(m_write);
            throw std::runtime_error("Failed to create process.");
        }
#elif defined(__linux__)
        //Fork
        if((m_pid = fork()) < 0)
        {
            std::cerr << getLastErrorAsString() << std::endl;
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
            int fd{ open(m_consoleFilePath.string().c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) };
            if(fd < 0)
            {
                std::cerr << getLastErrorAsString() << std::endl;
                throw std::runtime_error("Failed to create file.");
            }
            dup2(fd, STDERR_FILENO);
            dup2(fd, STDOUT_FILENO);
            //Create process
            raise(SIGSTOP);
            if(execvp(m_path.string().c_str(), appArgs.data()) < 0)
            {
                std::cerr << getLastErrorAsString() << std::endl;
                m_completed = true;
                m_exitCode = 1;
                exit(1);
            }
        }
#endif
    }

    Process::~Process()
    {
#ifdef _WIN32
        CloseHandle(m_read);
        CloseHandle(m_write);
        CloseHandle(m_pi.hProcess);
        CloseHandle(m_pi.hThread);
#elif defined(__linux__)
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
#elif defined(__linux__)
        waitpid(m_pid, nullptr, WUNTRACED);
        if(::kill(m_pid, SIGCONT) < 0)
#endif
        {
            std::cerr << getLastErrorAsString() << std::endl;
            return false;
        }
        m_watchThread = std::jthread(&Process::watch, this);
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
#ifdef _WIN32
        if(!TerminateProcess(m_pi.hProcess, 0))
#elif defined(__linux__)
        if(::kill(m_pid, SIGTERM) < 0)
#endif
        {
            std::cerr << getLastErrorAsString() << std::endl;
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
#ifdef _WIN32
        bool ended{ false };
        while(!ended)
        {
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
        }
        DWORD exitCode{ 0 };
        GetExitCodeProcess(m_pi.hProcess, &exitCode);
#elif defined(__linux__)
        int status{ 0 };
        bool ended{ false };
        while(!ended)
        {
            //Determine if ended
            while(waitpid(m_pid, &status, WNOHANG | WUNTRACED | WCONTINUED) > 0)
            {
                if(WIFEXITED(status) || WIFSIGNALED(status))
                {
                    ended = true;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        int exitCode{ WIFEXITED(status) ? WEXITSTATUS(status) : -1 };
        //Read console output
        std::ifstream file{ m_consoleFilePath };
        m_output = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
#endif
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_exitCode = static_cast<int>(exitCode);
        m_running = false;
        m_completed = true;
        lock.unlock();
        m_exited.invoke({ m_exitCode, m_output });
    }
}