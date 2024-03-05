#include "app/aura.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <curl/curl.h>
#include "filesystem/systemdirectories.h"
#include "helpers/stringhelpers.h"
#include "localization/gettext.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Nickvision::Filesystem;

namespace Nickvision::App
{
    Aura::Aura()
        : m_initialized{ false }
    {

    }

    Aura::~Aura()
    {
        if(m_initialized)
        {
            curl_global_cleanup();
        }
    }

    bool Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName)
    {
        if(!m_initialized)
        {
            //Get executable's directory path
#ifdef _WIN32
            wchar_t pth[MAX_PATH];
            DWORD len{ GetModuleFileNameW(nullptr, pth, sizeof(pth)) };
            if(len > 0)
            {
                m_executableDirectory = std::filesystem::path(std::wstring(pth, len)).parent_path();
            }
            else
            {
                throw std::runtime_error("Unable to get executable directory.");
            }
#elif defined(__linux__)
            m_executableDirectory = std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
            //Setup AppInfo
            m_appInfo.setId(id);
            m_appInfo.setName(name);
            m_appInfo.setEnglishShortName(englishShortName);
            //Setup curl
            if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0)
            {
                throw std::runtime_error("Unable to initialize curl.");
            }
            //Setup gettext
            std::string domainName{ StringHelpers::toLower(StringHelpers::replace(m_appInfo.getEnglishShortName(), " ", "")) };
            if (!Localization::Gettext::init(domainName))
            {
                throw std::runtime_error("Unable to initialize gettext.");
            }
            m_initialized = true;
        }
        return m_initialized;
    }

    const std::filesystem::path& Aura::getExecutableDirectory() const
    {
        return m_executableDirectory;
    }

    bool Aura::isRunningOnWindows() const
    {
#ifdef _WIN32
        return true;
#else
        return false;
#endif
    }

    bool Aura::isRunningOnLinux() const
    {
#ifdef __linux__
        return true;
#else
        return false;
#endif
    }

    bool Aura::isRunningViaFlatpak() const
    {
        return std::filesystem::exists("/.flatpak-info");
    }

    bool Aura::isRunningViaSnap() const
    {
        return !getEnvVar("SNAP").empty();
    }

    bool Aura::isRunningViaLocal() const
    {
        return !isRunningViaFlatpak() && !isRunningViaSnap();
    }

    AppInfo& Aura::getAppInfo()
    {
        return m_appInfo;
    }

    InterProcessCommunicator& Aura::getIPC()
    {
        if(!m_ipc)
        {
            m_ipc = std::make_unique<InterProcessCommunicator>(m_appInfo.getId());
        }
        return *m_ipc;
    }

    std::string Aura::getEnvVar(const std::string& key) const
    {
        char* var{ std::getenv(key.c_str()) };
        if (var)
        {
            return { var };
        }
        return "";
    }

    bool Aura::setEnvVar(const std::string& key, const std::string& value)
    {
#ifdef _WIN32
        return _putenv_s(key.c_str(), value.c_str()) == 0;
#elif defined(__linux__)
        return setenv(key.c_str(), value.c_str(), true) == 0;
#endif
    }

    std::string Aura::sysExec(const std::string& command)
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
            STARTUPINFOA si{ 0 };
            PROCESS_INFORMATION pi{ 0 };
            std::string commandCopy{ "cmd.exe /C \"" + command + "\"" };
            si.cb = sizeof(STARTUPINFOA);
            si.hStdError = write;
            si.hStdOutput = write;
            si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            si.wShowWindow = SW_HIDE;
            if(CreateProcessA(nullptr, LPSTR(commandCopy.c_str()), nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi))
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

    const std::filesystem::path& Aura::findDependency(std::string dependency)
    {
#ifdef _WIN32
        if (!std::filesystem::path(dependency).has_extension())
        {
            dependency += ".exe";
        }
#endif
        if (m_dependencies.contains(dependency))
        {
            const std::filesystem::path& location{ m_dependencies[dependency] };
            if (std::filesystem::exists(location))
            {
                return location;
            }
        }
        m_dependencies[dependency] = std::filesystem::path();
        std::filesystem::path path{ getExecutableDirectory() / dependency };
        if (std::filesystem::exists(path))
        {
            m_dependencies[dependency] = path;
        }
        else
        {
            for (const std::filesystem::path& dir : SystemDirectories::getPath())
            {
                path = { dir / dependency };
                if (std::filesystem::exists(path) && dir.string().find("AppData\\Local\\Microsoft\\WindowsApps") == std::string::npos)
                {
                    m_dependencies[dependency] = path;
                    break;
                }
            }
        }
        return m_dependencies[dependency];
    }

    Aura& Aura::getActive()
    {
        static Aura aura;
        return aura;
    }
}