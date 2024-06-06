#include "localization/documentation.h"
#include <locale>
#include "app/aura.h"
#include "helpers/stringhelpers.h"
#include "localization/gettext.h"
#include "system/environment.h"

using namespace Nickvision::App;
using namespace Nickvision::System;

namespace Nickvision::Localization
{
    std::string Documentation::getHelpUrl(const std::string& pageName)
    {
#ifdef __linux__
        if (Environment::getVariable("SNAP").empty())
        {
            return "help:" + StringHelpers::toLower(Aura::getActive().getAppInfo().getEnglishShortName()) + "/" + pageName;
        }
#endif
        std::string lang{ "C" };
        std::string sysLocale{ StringHelpers::split(std::locale("").name(), ".")[0] }; //split to remove the .UTF-8
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
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(Aura::getActive().getExecutableDirectory()))
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
        return "https://htmlpreview.github.io/?" + Aura::getActive().getAppInfo().getHtmlDocsStore() + "/" + lang + "/" + pageName + ".html";
    }
}