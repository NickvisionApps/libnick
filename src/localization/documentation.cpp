#include "localization/documentation.h"
#include "localization/gettext.h"
#include "helpers/stringhelpers.h"
#include "system/environment.h"

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Localization
{
    std::string Documentation::getHelpUrl(const std::string& englishShortName, const std::string& htmlDocStore, const std::string& pageName)
    {
#ifdef __linux__
        if (Environment::getDeploymentMode() != DeploymentMode::Snap)
        {
            return "help:" + StringHelpers::lower(englishShortName) + "/" + pageName;
        }
#endif
        std::string lang{ "C" };
        std::string sysLocale{ Environment::getLocaleName() };
#ifdef _WIN32
        sysLocale = StringHelpers::replace(sysLocale, '-', '_');
#else
        sysLocale = StringHelpers::split(sysLocale, ".")[0]; //split to remove the .UTF-8
#endif
        if (!sysLocale.empty() && sysLocale != "C" && sysLocale != "en_US" && sysLocale != "*")
        {
            std::string twoLetter{ StringHelpers::split(sysLocale, "_")[0] };
            for(const std::string& l : Gettext::getAvailableLanguages())
            {
                if(l == sysLocale || l == twoLetter)
                {
                    lang = l;
                    break;
                }
            }
        }
        return "https://htmlpreview.github.io/?" + htmlDocStore + "/" + lang + "/" + pageName + ".html";
    }
}
