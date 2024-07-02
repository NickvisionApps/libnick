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
 * A random password generator.
 */

#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <vector>
#include "passwordcontent.h"

namespace Nickvision::Keyring
{
    /**
     * @brief A random password generator. 
     */
    class PasswordGenerator
    {
    public:
        /**
         * @brief Constructs a PasswordGenerator.
         * @param contentFlags Flags of possible characters in a generated password
         */
        PasswordGenerator(PasswordContent contentFlags = PasswordContent::Numeric | PasswordContent::Uppercase | PasswordContent::Lowercase | PasswordContent::Special);
        /**
         * @brief Gets the flags of possible characters in a generated password.
         * @return The flags of possible characters in a generated password
         */
        PasswordContent getContentFlags() const;
        /**
         * @brief Sets the flags of possible characters in a generated password.
         * @param contentFlags Flags of possible characters in a generated password
         */
        void setContentFlags(PasswordContent contentFlags);
        /**
         * @brief Generates a new password
         * @param length The length of the generated password
         * @return The generated password
         */
        std::string next(size_t length = 16);

    private:
        PasswordContent m_contentFlags;
    };
}

#endif //PASSWORDGENERATOR_H