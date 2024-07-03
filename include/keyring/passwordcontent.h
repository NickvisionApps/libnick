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
        Special = 8 ///< The password contains special characters.
    };

    constexpr PasswordContent operator~(PasswordContent a)
    {
        return static_cast<PasswordContent>(~static_cast<int>(a));
    }

    constexpr PasswordContent operator|(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) | static_cast<int>(b));
    }

    constexpr PasswordContent operator&(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr PasswordContent operator^(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    inline PasswordContent& operator|=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) |= static_cast<int>(b));
    }

    inline PasswordContent& operator&=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) &= static_cast<int>(b));
    }

    inline PasswordContent& operator^=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b));
    }
}

#endif //PASSWORDCONTENT_H