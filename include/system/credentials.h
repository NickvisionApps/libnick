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
 * Functions for working with the system credential manager (secret store).
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <optional>
#include <string>
#include "keyring/credential.h"

namespace Nickvision::System::Credentials
{
    /**
     * @brief Gets a credential from the system's credential manager.
     * @param name The name of the credential
     * @return The Credential object
     * @return std::nullopt if the credential was not found
     */
    std::optional<Keyring::Credential> get(const std::string& name) noexcept;
    /**
     * @brief Creates a new credential with a random password and adds it to the system's credential manager.
     * @param name The name of the credential to create
     * @return The new Credential object
     * @return std::nullopt if the credential could not be created
     */
    std::optional<Keyring::Credential> create(const std::string& name) noexcept;
    /**
     * @brief Adds a new credential object to the system's credential manager. 
     * @brief On Linux, only the name and password of a credential object will be stored. 
     * @brief On Windows, all fields of a credential object will be stored.
     * @brief On macOS, all fields of a credential object will be stored.
     * @param credential The new credential object
     * @return True if the credential object was added
     * @return False if the credential object was not added
     */
    bool add(const Keyring::Credential& credential) noexcept;
    /**
     * @brief Updates a credential object in the system's credential manager.
     * @brief On Linux, only the name and password of a credential object will be stored. 
     * @brief On Windows, all fields of a credential object will be stored.
     * @brief On macOS, all fields of a credential object will be stored.
     * @param credential The updated credential object
     * @return True if the credential object was updated
     * @return False if the credential object was not updated
     */
    bool update(const Keyring::Credential& credential) noexcept;
    /**
     * @brief Removes a credential object from the system's credential manager.
     * @param name The name of the credential to remove
     * @return True if the credential object was removed
     * @return False if the credential object was not removed
     */
    bool remove(const std::string& name) noexcept;
}

#endif //SYSTEMCREDENTIALS_H