#include "system/environment.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include "helpers/stringhelpers.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::System
{
    std::string Environment::getVariable(const std::string& key)
    {
        char* var{ std::getenv(key.c_str()) };
        if (var)
        {
            return { var };
        }
        return "";
    }

    bool Environment::setVariable(const std::string& key, const std::string& value)
    {
#ifdef _WIN32
        return _putenv_s(key.c_str(), value.c_str()) == 0;
#elif defined(__linux__)
        return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
    }

    bool Environment::clearVariable(const std::string& key)
    {
        return setVariable(key, "");
    }

    std::string Environment::exec(const std::string& command)
    {
        std::string result;
#ifdef _WIN32
        SECURITY_ATTRIBUTES sa{ 0 };
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = nullptr;
        HANDLE read;
        HANDLE write;
        if(CreatePipe(&read, &write, &sa, 0))
        {
            STARTUPINFOW si{ 0 };
            PROCESS_INFORMATION pi{ 0 };
            std::wstring commandCopy{ L"cmd.exe /C \"" + StringHelpers::toWstring(command) + L"\"" };
            si.cb = sizeof(STARTUPINFOW);
            si.hStdError = write;
            si.hStdOutput = write;
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            si.wShowWindow = SW_HIDE;
            if(CreateProcessW(nullptr, LPWSTR(commandCopy.c_str()), nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi))
            {
                bool ended{ false };
                while(!ended)
                {
                    ended = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;
                    while(true)
                    {
                        std::array<char, 1024> buffer;
                        DWORD bytes{ 0 };
                        DWORD available{ 0 };
                        if(!PeekNamedPipe(read, nullptr, 0, nullptr, &available, nullptr))
                        {
                            break;
                        }
                        if(!available)
                        {
                            break;
                        }
                        if(!ReadFile(read, buffer.data(), min(static_cast<int>(buffer.size()) - 1, available), &bytes, nullptr) || !bytes)
                        {
                            break;
                        }
                        buffer[bytes] = 0;
                        result += buffer.data();
                    }
                }
                CloseHandle(read);
                CloseHandle(write);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return result;
            }
            else
            {
                CloseHandle(read);
                CloseHandle(write);
            }
        }
#elif defined(__linux__)
        std::array<char, 128> buffer;
        std::unique_ptr<FILE, decltype(&pclose)> pipe{ popen(command.c_str(), "r"), pclose };
        if (pipe)
        {
            while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
            {
                result += buffer.data();
            }
            return result;
        }
#endif
        return "";
    }
}