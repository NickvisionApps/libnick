#include "localization/gettext.h"
#include <cstdlib>
#include <filesystem>
#include "helpers/stringhelpers.h"
#include "system/environment.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Localization
{
    static std::string s_domainName;

    bool Gettext::init(const std::string& domainName)
    {
        static bool initialized{ false };
        if(!initialized)
        {
            bool res{ true };
            setlocale(LC_ALL, "");
            s_domainName = StringHelpers::lower(StringHelpers::replace(domainName, " ", ""));
#ifdef _WIN32
            res = res && (wbindtextdomain(s_domainName.c_str(), Environment::getExecutableDirectory().c_str()) != nullptr);
#else
            res = res && (bindtextdomain(s_domainName.c_str(), Environment::getExecutableDirectory().c_str()) != nullptr);
#endif
            res = res && (bind_textdomain_codeset(s_domainName.c_str(), "UTF-8") != nullptr);
            res = res && (textdomain(s_domainName.c_str()) != nullptr);
            initialized = true;
            return res;
        }
        return true;
    }

    const std::string& Gettext::getDomainName()
    {
        return s_domainName;
    }

    const std::vector<std::string>& Gettext::getAvailableLanguages()
    {
        static std::vector<std::string> langs;
        if(langs.empty())
        {
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(Environment::getExecutableDirectory()))
            {
                if (e.is_directory() && std::filesystem::exists(e.path() / (getDomainName() + ".mo")))
                {
                    langs.push_back(e.path().filename().string());
                }
            }
        }
        return langs;
    }

    bool Gettext::changeLanguage(const std::string& language)
    {
        const std::vector<std::string>& langs{ Gettext::getAvailableLanguages() };
        if (std::find(langs.begin(), langs.end(), language) == langs.end())
        {
            return false;
        }
        Environment::setVariable("LANGUAGE", language);
        return true;
    }

    const char* Gettext::pgettext(const char* context, const char* msg)
    {
        const char* translation{ dcgettext(s_domainName.c_str(), context, LC_MESSAGES) };
        if (translation == context)
        {
            return msg;
        }
        return translation;
    }

    const char* Gettext::pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n)
    {
        const char* translation{ dcngettext(s_domainName.c_str(), context, msgPlural, n, LC_MESSAGES) };
        if (translation == context || translation == msgPlural)
        {
            return n == 1 ? msg : msgPlural;
        }
        return translation;
    }
}
