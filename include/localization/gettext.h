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

#include <string>
#include <libintl.h>

#define GETTEXT_CONTEXT_SEPARATOR "\004"
#define _(String) dgettext(::Nickvision::Localization::Gettext::getDomainName().c_str(), String)
#define _n(String, StringPlural, N) dngettext(::Nickvision::Localization::Gettext::getDomainName().c_str(), String, StringPlural, static_cast<unsigned long>(N))
#define _p(Context, String) ::Nickvision::Localization::Gettext::pgettext(Context GETTEXT_CONTEXT_SEPARATOR String, String)
#define _pn(Context, String, StringPlural, N) ::Nickvision::Localization::Gettext::pngettext(Context GETTEXT_CONTEXT_SEPARATOR String, String, StringPlural, static_cast<unsigned long>(N))

namespace Nickvision::Localization::Gettext
{
    /**
     * @brief Initializes the gettext system. This function should only be called once, regardless of with different domain names.
     * @param domainName The domain name to use for gettext translations. Must be lowercase and contain no spaces.
     * @return True if initialized, else false
     */
    bool init(const std::string& domainName);
    /**
     * @brief Gets the domain name used for gettext translations.
     * @return The gettext domain name
     */
    const std::string& getDomainName();
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