#include "localization/gettext.h"
#include <filesystem>
#include "aura.h"
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{
	bool Localization::init()
	{
		std::string name{ StringHelpers::toLower(StringHelpers::replace(Aura::getActive().getAppInfo().getEnglishShortName(), " ", "")) };
		if (name.empty())
		{
			name = StringHelpers::toLower(StringHelpers::replace(Aura::getActive().getAppInfo().getName(), " ", ""));
		}
		bindtextdomain(name.c_str(), std::filesystem::current_path().string().c_str());
		bind_textdomain_codeset(name.c_str(), "UTF-8");
		textdomain(name.c_str());
	}
}