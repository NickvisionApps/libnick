/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Functions for working with gettext (libintl) translations.
 */

#ifndef GETTEXT_H
#define GETTEXT_H

#include <format>
#include <string>
#include <vector>
#include <libintl.h>

#define GETTEXT_CONTEXT_SEPARATOR "\004"
#define _(String) ::Nickvision::Localization::Gettext::dgettext(String)
#define _n(String, StringPlural, N) ::Nickvision::Localization::Gettext::dngettext(String, StringPlural, static_cast<unsigned long>(N))
#define _f(String, ...) ::Nickvision::Localization::Gettext::fgettext(String, __VA_ARGS__)
#define _fn(String, StringPlural, N, ...) ::Nickvision::Localization::Gettext::fngettext(String, StringPlural, static_cast<unsigned long>(N), __VA_ARGS__)
#define _p(Context, String) ::Nickvision::Localization::Gettext::pgettext(Context GETTEXT_CONTEXT_SEPARATOR String, String)
#define _pn(Context, String, StringPlural, N) ::Nickvision::Localization::Gettext::pngettext(Context GETTEXT_CONTEXT_SEPARATOR String, String, StringPlural, static_cast<unsigned long>(N))

namespace Nickvision::Localization::Gettext
{
    /**
     * @brief Initializes the gettext system. This function should only be called once, regardless of with different domain names.
     * @param domainName The domain name to use for gettext translations. Must be lowercase and contain no spaces
     * @return True if initialized, else false
     */
    bool init(const std::string& domainName);
    /**
     * @brief Gets the domain name used for gettext translations.
     * @return The gettext domain name
     */
    const std::string& getDomainName();
    /**
     * @brief Gets the list of available translated languages.
     * @brief Does not include the "C" language (the default language).
     * @return The list of available translated languages.
     */
    const std::vector<std::string>& getAvailableLanguages();
    /**
     * @brief Changes the current language for gettext translations.
     * @param language The language code to change translations to (use "C" to turn off translations; use "" to use the system default language)
     * @return True if the language was changed successfully, else false
     */
    bool changeLanguage(const std::string& language);
    /**
     * @brief Translates a message.
     * @param msgid The message to translate
     * @return The translated message
     */
    const char* dgettext(const char* msgid);
    /**
     * @brief Translates a plural message.
     * @param msg The message to translate
     * @param msgPlural The plural version of the message to translate
     * @param n The number of objects (used to determine whether or not to use the plural version of the message)
     * @return The translated message for the given number of objects
     */
    const char* dngettext(const char* msg, const char* msgPlural, unsigned long n);
    /**
     * @brief Translates a message and formats it with the given arguments.
     * @param msg The message to translate
     * @param args The arguments to format the translated message with
     * @return The formatted translated message
     */
    template<typename... Args>
    std::string fgettext(const char* msg, Args&&... args)
    {
        return std::vformat(Nickvision::Localization::Gettext::dgettext(msg), std::make_format_args(args...));
    }
    /**
     * @brief Translates a plural message and formats it with the given arguments.
     * @param msg The message to translate
     * @param msgPlural The plural version of the message to translate
     * @param n The number of objects (used to determine whether or not to use the plural version of the message)
     * @param args The arguments to format the translated message with
     * @return The formatted translated message for the given number of objects
     */
    template<typename... Args>
    std::string fngettext(const char* msg, const char* msgPlural, unsigned long n, Args&&... args)
    {
        return std::vformat(Nickvision::Localization::Gettext::dngettext(msg, msgPlural, n), std::make_format_args(args...));
    }
    /**
     * @brief Translates a message for a given context.
     * @param context The context of the message
     * @param msg The message to translate
     * @return The translated message for the given context.
     */
    const char* pgettext(const char* context, const char* msg);
    /**
     * @brief Translates a plural message for a given context.
     * @param context The context of the message
     * @param msg The message to translate
     * @param msgPlural The plural version of the message to translate
     * @param n The number of objects (used to determine whether or not to use the plural version of the message)
     * @return The translated message for the given context and number of objects.
     */
    const char* pngettext(const char* context, const char* msg, const char* msgPlural, unsigned long n);
}

#endif //GETTEXT_H
