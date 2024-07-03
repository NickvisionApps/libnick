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
 * Strengths for a password.
 */

#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

#include <string>

namespace Nickvision::Keyring
{
    /**
     * @brief Strengths for a password. 
     */
    enum class PasswordStrength
    {
        Blank = 0, ///< A blank password.
        VeryWeak, ///< A very weak password.
        Weak, ///< A weak password.
        Medium, ///< A medium password.
        Strong, ///< A strong password.
        VeryStrong ///< A very strong password.
    };

    /**
     * @brief Calculates the strength of a given password.
     * @param password The password to measure the strength of
     * @return The strength of the password
     */
    PasswordStrength getPasswordStrength(const std::string& password);
}

#endif //PASSWORDSTRENGTH_H