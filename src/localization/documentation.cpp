#include "localization/documentation.h"
#include <boost/locale.hpp>
#include "aura/aura.h"
#include "helpers/stringhelpers.h"

namespace Nickvision::Localization
{
	std::string Documentation::getHelpUrl(const std::string& pageName) noexcept
	{
#ifdef __linux__
		if (Aura::getEnvVar("SNAP").empty())
		{
			return "help:" + StringHelpers::toLower(Aura::getActive().getAppInfo().getEnglishShortName()) + "/" + pageName;
		}
#endif
		std::string lang{ "C" };
		std::string sysLocale{ StringHelpers::split(boost::locale::util::get_system_locale(), ".")[0] }; //split to remove the .UTF-8
		if (!sysLocale.empty() && sysLocale != "C" && sysLocale != "en_US")
		{
			/*
			 * Because the translations of a Nickvision application are generated and stored in the application's 
			 * running directory, we can look at the list of directory names and see if they match the sysLocale. 
			 * For example, if there is a Brazilian Portuguese translation, there will be a folder called pt_BR.
			 */
			std::vector<std::string> langs;
			for (const std::filesystem::directory_entry& e : std::filesystem::directory_iterator(std::filesystem::current_path()))
			{
				if (e.is_directory())
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
		return "https://htmlpreview.github.io/?" + Aura::Aura::getActive().getAppInfo().getHtmlDocsStore() + "/" + lang + "/" + pageName + ".html";
	}
}