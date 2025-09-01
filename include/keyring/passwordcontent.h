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
 * Flags to describe the content of a password.
 */

#ifndef PASSWORDCONTENT_H
#define PASSWORDCONTENT_H

#include "helpers/codehelpers.h"

namespace Nickvision::Keyring
{
    /**
     * @brief Flags to describe the content of a password.
     */
    enum class PasswordContent
    {
        Numeric = 1, ///< The password contains numeric characters.
        Uppercase = 2, ///< The password contains uppercase characters.
        Lowercase = 4, ///< The password contains lowercase characters.
        Special = 8, ///< The password contains special characters.
        Space = 16 ///< The password contains spaces.
    };

    DEFINE_ENUM_FLAGS(PasswordContent)
}

#endif //PASSWORDCONTENT_H