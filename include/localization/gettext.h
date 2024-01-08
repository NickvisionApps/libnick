#ifndef GETTEXT_H
#define GETTEXT_H

#include <string>
#include <libintl.h>

#define GETTEXT_CONTEXT_SEPARATOR "\004"
#define _(String) gettext(String)
#define _n(String, StringPlural, N) ngettext(String, StringPlural, N)
#define _p(Context, String) ::Nickvision::Localization::Gettext::pgettext(Context GETTEXT_CONTEXT_SEPARATOR String, String)
#define _pn(Context, String, StringPlural, N) ::Nickvision::Localization::Gettext::pngettext(Context GETTEXT_CONTEXT_SEPARATOR String, String, StringPlural, N)

namespace Nickvision::Localization::Gettext
{
	/**
	 * @brief Initializes the gettext system. This function should only be called once, regradless of with different domain names.
	 * @param domainName The domain name to use for gettext translations
	 * @return True if initialized, else false
	 */
	bool init(const std::string& domainName) noexcept;
	/**
	 * @brief Translates a message for a given context.
	 * @param context The context of the message
	 * @param msg The message to translate
	 * @return The translated message for the given context.
	 */
	const char* pgettext(const char* context, const char* msg) noexcept;
	/**
	 * @brief Translates a plural message for a given context.
	 * @param context The context of the message
	 * @param msg The message to translate
	 * @param msgPlural The plural version of the message to translate
	 * @param n The number of objects (used to determine whether or not to use the plural version of the message)
	 * @return The translated message for the given context and number of objects.
	 */
	const char* pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n) noexcept;
}

#endif //GETTEXT_H