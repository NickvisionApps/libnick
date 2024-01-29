#include "localization/documentation.h"
#include <boost/locale.hpp>
#include "app/aura.h"
#include "helpers/stringhelpers.h"
#include "localization/gettext.h"

using namespace Nickvision::App;

namespace Nickvision::Localization
{
    std::string Documentation::getHelpUrl(const std::string& pageName)
    {
#ifdef __linux__
        if (Aura::getActive().getEnvVar("SNAP").empty())
        {
            return "help:" + StringHelpers::toLower(Aura::getActive().getAppInfo().getEnglishShortName()) + "/" + pageName;
        }
#endif
        std::string lang{ "C" };
        std::string sysLocale{ StringHelpers::split(boost::locale::util::get_system_locale(), ".")[0] }; //split to remove the .UTF-8
        if (!sysLocale.empty() && sysLocale != "C" && sysLocale != "en_US")
        {
            /*
             * Because the translations of a Nickvision application are stored in the application's running
             * directory, we can look at the list of directory names and see if they contain a translation
             * file (.mo). If a directory contains an mo file, we can add that directory name (which is a 
             * language code) to the list of available translated languages for the app.
             */
            std::vector<std::string> langs;
            for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(Aura::getActive().getExecutableDirectory()))
            {
                if (e.is_directory() && std::filesystem::exists(e.path() / (Gettext::getDomainName() + ".mo")))
                {
                    langs.push_back(e.path().filename().string());
                }
            }
            if (std::find(langs.begin(), langs.end(), sysLocale) != langs.end())
            {
                lang = sysLocale;
            }
            else
            {
                std::string twoLetter{ StringHelpers::split(sysLocale, "_")[0] };
                for (const std::string& l : langs)
                {
                    if (l.find(twoLetter) != std::string::npos)
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