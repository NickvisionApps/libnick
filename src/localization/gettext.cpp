#include "localization/gettext.h"
#include <filesystem>
#include <cstdlib>
#include "app/aura.h"

using namespace Nickvision::App;

namespace Nickvision::Localization
{
    static std::string m_domainName;

    bool Gettext::init(const std::string& domainName)
    {
        static bool initialized{ false };
        if(!initialized)
        {
            bool res{ true };
            setlocale(LC_ALL, "");
            m_domainName = domainName;
#ifdef _WIN32
            res = res && (wbindtextdomain(m_domainName.c_str(), Aura::getActive().getExecutableDirectory().c_str()) != nullptr);
#else
            res = res && (bindtextdomain(m_domainName.c_str(), Aura::getActive().getExecutableDirectory().c_str()) != nullptr);
            res = res && (bind_textdomain_codeset(m_domainName.c_str(), "UTF-8") != nullptr);
#endif
            res = res && (textdomain(m_domainName.c_str()) != nullptr);
            initialized = true;
            return res;
        }
        return true;
    }

    const std::string& Gettext::getDomainName()
    {
        return m_domainName;
    }

    const char* Gettext::pgettext(const char* context, const char* msg)
    {
        const char* translation{ dcgettext(m_domainName.c_str(), context, LC_MESSAGES) };
        if (translation == context)
        {
            return msg;
        }
        return translation;
    }

    const char* Gettext::pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n)
    {
        const char* translation{ dcngettext(m_domainName.c_str(), context, msgPlural, n, LC_MESSAGES) };
        if (translation == context || translation == msgPlural)
        {
            return n == 1 ? msg : msgPlural;
        }
        return translation;
    }
}