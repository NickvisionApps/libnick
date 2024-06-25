#include "app/aura.h"
#include <locale>
#include "filesystem/userdirectories.h"
#include "helpers/stringhelpers.h"
#include "localization/gettext.h"
#include "system/environment.h"

using namespace Nickvision::Filesystem;
using namespace Nickvision::Helpers;
using namespace Nickvision::Localization;
using namespace Nickvision::Logging;
using namespace Nickvision::System;

namespace Nickvision::App
{
    Aura::Aura()
        : m_initialized{ false }
    {

    }

    bool Aura::init(const std::string& id, const std::string& name, const std::string& englishShortName, LogLevel logLevel)
    {
        if(!m_initialized)
        {
            //Setup AppInfo
            m_appInfo.setId(id);
            m_appInfo.setName(name);
            m_appInfo.setEnglishShortName(englishShortName);
            //Setup gettext
            std::string domainName{ StringHelpers::lower(StringHelpers::replace(m_appInfo.getEnglishShortName(), " ", "")) };
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

    std::string Aura::getHelpUrl(const std::string& pageName)
    {
#ifdef __linux__
        if (Environment::getVariable("SNAP").empty())
        {
            return "help:" + StringHelpers::lower(m_appInfo.getEnglishShortName()) + "/" + pageName;
        }
#endif
        std::string lang{ "C" };
        std::string sysLocale{ Environment::getLocaleName() };
#ifdef _WIN32
        sysLocale = StringHelpers::replace(sysLocale, "-", "_");
#else
        sysLocale = StringHelpers::split(sysLocale, ".")[0]; //split to remove the .UTF-8
#endif
        if (!sysLocale.empty() && sysLocale != "C" && sysLocale != "en_US" && sysLocale != "*")
        {
            /*
            * Because the translations of a Nickvision application are stored in the application's running
            * directory, we can look at the list of directory names and see if they contain a translation
            * file (.mo) for the sysLocale string. If a directory contains an mo file, we can set the 
            * documentation lang to the sysLocale. If no directory contains an mo file for the sysLocale, 
            * we can try to match using the two-letter language code. If that doesn't work, the default
            * English docs will be used.
            */
            std::string twoLetter{ StringHelpers::split(sysLocale, "_")[0] };
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(Environment::getExecutableDirectory()))
            {
                if (e.is_directory() && std::filesystem::exists(e.path() / (Gettext::getDomainName() + ".mo")))
                {
                    std::string l{ e.path().filename().string() };
                    if(l == sysLocale || l == twoLetter)
                    {
                        lang = l;
                        break;
                    }
                }
            }
        }
        return "https://htmlpreview.github.io/?" + m_appInfo.getHtmlDocsStore() + "/" + lang + "/" + pageName + ".html";
    }

#ifdef _WIN32
    Notifications::NotifyIcon& Aura::getNotifyIcon(HWND hwnd)
    {
        if (!m_notifyIcon)
        {
            m_notifyIcon = std::make_unique<Notifications::NotifyIcon>(hwnd, StringHelpers::wstr(m_appInfo.getName()), Notifications::NotifyIconMenu{}, true);
        }
        return *m_notifyIcon;
    }
#endif

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
