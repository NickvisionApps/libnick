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

#include <random>
#include <string>
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
         * @param contentFlags Flags of possible characters for the generator to use
         */
        PasswordGenerator(PasswordContent contentFlags = PasswordContent::Numeric | PasswordContent::Uppercase | PasswordContent::Lowercase | PasswordContent::Special | PasswordContent::Space) noexcept;
        /**
         * @brief Gets the flags of possible characters for the generator to use.
         * @return The flags of possible characters for the generator to use
         */
        PasswordContent getContentFlags() const noexcept;
        /**
         * @brief Sets the flags of possible characters for the generator to use.
         * @param contentFlags Flags of possible characters for the generator to use
         */
        void setContentFlags(PasswordContent contentFlags) noexcept;
        /**
         * @brief Generates a new password
         * @param length The length of the generated password
         * @return The generated password
         */
        std::string next(size_t length = 16) noexcept;

    private:
        PasswordContent m_contentFlags;
        std::random_device m_randomDevice;
        std::mt19937 m_randomEngine;
    };
}

#endif //PASSWORDGENERATOR_H