#include "localization/gettext.h"
#include <filesystem>
#include <cstdlib>

namespace Nickvision::Aura::Localization
{
	bool Gettext::init(const std::string& domainName) noexcept
	{
		bool res{ true };
		setlocale(LC_ALL, "");
#ifdef _WIN32
		res = res && (wbindtextdomain(domainName.c_str(), std::filesystem::current_path().c_str()) != nullptr);
#elif defined(__linux__)
		res = res && (bindtextdomain(domainName.c_str(), std::filesystem::current_path().c_str()) != nullptr);
		res = res && (bind_textdomain_codeset(domainName.c_str(), "UTF-8") != nullptr);
#endif
		res = res && (textdomain(domainName.c_str()) != nullptr);
		return res;
	}

	const char* Gettext::pgettext(const char* context, const char* msg) noexcept
	{
		const char* translation{ dcgettext(nullptr, context, LC_MESSAGES) };
		if (translation == context)
		{
			return msg;
		}
		return translation;
	}

	const char* Gettext::pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n) noexcept
	{
		const char* translation{ dcngettext(nullptr, context, msgPlural, n, LC_MESSAGES) };
		if (translation == context || translation == msgPlural)
		{
			return n == 1 ? msg : msgPlural;
		}
		return translation;
	}
}