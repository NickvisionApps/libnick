#include "app/aura.h"
#include <curl/curl.h>
#include "filesystem/systemdirectories.h"
#include "filesystem/userdirectories.h"
#include "helpers/stringhelpers.h"
#include "localization/gettext.h"
#include "system/environment.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace Nickvision::Filesystem;
using namespace Nickvision::Logging;
using namespace Nickvision::System;

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

    bool Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName, LogLevel logLevel)
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
            //Setup logger
            std::filesystem::path logPath{ UserDirectories::getApplicationCache() / "log.txt" };
            if(std::filesystem::exists(logPath))
            {
                std::filesystem::remove(logPath);
            }
            m_logger = std::make_unique<Logger>(logPath, logLevel);
            m_initialized = true;
        }
        return m_initialized;
    }

    bool Aura::isValid() const
    {
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
        return !Environment::getVariable("SNAP").empty();
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

    const Logger& Aura::getLogger() const
    {
        return *m_logger;
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

    Aura::operator bool() const
    {
        return isValid();
    }

    Aura& Aura::getActive()
    {
        static Aura aura;
        return aura;
    }
}
