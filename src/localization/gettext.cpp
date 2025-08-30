#include "localization/gettext.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include "helpers/stringhelpers.h"
#include "system/environment.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Localization
{
    static std::string s_domainName{};
    static bool s_translationsOff{ false };

    bool Gettext::init(const std::string& domainName) noexcept
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

    const std::string& Gettext::getDomainName() noexcept
    {
        return s_domainName;
    }

    const std::vector<std::string>& Gettext::getAvailableLanguages() noexcept
    {
        static std::vector<std::string> langs;
        if(langs.empty())
        {
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(Environment::getExecutableDirectory()))
            {
                if (e.is_directory() && std::filesystem::exists(e.path() / "LC_MESSAGES" / (getDomainName() + ".mo")))
                {
                    langs.push_back(e.path().filename().string());
                }
            }
            std::sort(langs.begin(), langs.end());
        }
        return langs;
    }

    bool Gettext::changeLanguage(const std::string& language) noexcept
    {
        if(language.empty())
        {
            if(Environment::hasVariable("LANGUAGE"))
            {
                Environment::clearVariable("LANGUAGE");
            }
            s_translationsOff = false;
            return true;
        }
        else if(language == "C")
        {
            s_translationsOff = true;
            return true;
        }
        const std::vector<std::string>& langs{ Gettext::getAvailableLanguages() };
        if (std::find(langs.begin(), langs.end(), language) == langs.end())
        {
            return false;
        }
        Environment::setVariable("LANGUAGE", language);
        s_translationsOff = false;
        return true;
    }

    const char* Gettext::dgettext(const char* msgid) noexcept
    {
        if(s_translationsOff)
        {
            return msgid;
        }
        return ::dgettext(s_domainName.c_str(), msgid);
    }

    const char* Gettext::dngettext(const char* msg, const char* msgPlural, unsigned long n) noexcept
    {
        if(s_translationsOff)
        {
            return n == 1 ? msg : msgPlural;
        }
        return ::dngettext(s_domainName.c_str(), msg, msgPlural, n);
    }

    const char* Gettext::pgettext(const char* context, const char* msg) noexcept
    {
        if(s_translationsOff)
        {
            return msg;
        }
        const char* translation{ ::dcgettext(s_domainName.c_str(), context, LC_MESSAGES) };
        if (translation == context)
        {
            return msg;
        }
        return translation;
    }

    const char* Gettext::pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n) noexcept
    {
        if(s_translationsOff)
        {
            return n == 1 ? msg : msgPlural;
        }
        const char* translation{ ::dcngettext(s_domainName.c_str(), context, msgPlural, n, LC_MESSAGES) };
        if (translation == context || translation == msgPlural)
        {
            return n == 1 ? msg : msgPlural;
        }
        return translation;
    }
}
