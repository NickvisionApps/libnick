#include "localization/gettext.h"
#include <filesystem>
#include "aura.h"
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura
{
	bool Localization::init()
	{
		bool res{ true };
		std::string name{ StringHelpers::toLower(StringHelpers::replace(Aura::getActive().getAppInfo().getEnglishShortName(), " ", "")) };
		if (name.empty())
		{
			name = StringHelpers::toLower(StringHelpers::replace(Aura::getActive().getAppInfo().getName(), " ", ""));
		}
		res &= bindtextdomain(name.c_str(), std::filesystem::current_path().string().c_str()) != nullptr;
		res &= bind_textdomain_codeset(name.c_str(), "UTF-8") != nullptr;
		res &= textdomain(name.c_str()) != nullptr;
		return res;
	}
}