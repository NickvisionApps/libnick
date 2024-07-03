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
 * Functions for working with the system credential manager.
 */

#ifndef SYSTEMCREDENTIALS_H
#define SYSTEMCREDENTIALS_H

#include <optional>
#include <string>
#include "credential.h"

namespace Nickvision::Keyring::SystemCredentials
{
    /**
     * @brief Gets a credential from the system's credential manager.
     * @param name The name of the credential
     * @return The Credential object, if found
     */
    std::optional<Credential> getCredential(const std::string& name);
    /**
     * @brief Adds a new credential with a random password to the system's credential manager.
     * @param name The name of the credential
     * @return The new Credential object, if successful
     */
    std::optional<Credential> addCredential(const std::string& name);
    /**
     * @brief Adds a new credential to the system's credential manager. 
     * @brief On Linux, only the name and password of a credential will be stored. 
     * @brief On Windows, all fields of a credential will be stored.
     * @brief On macOS, all fields of a credential will be stored.
     * @param credential The new credential object
     * @return True if successful, else false
     */
    bool addCredential(const Credential& credential);
    /**
     * @brief Updates a credential in the system's credential manager.
     * @brief On Linux, only the name and password of a credential will be stored. 
     * @brief On Windows, all fields of a credential will be stored.
     * @brief On macOS, all fields of a credential will be stored.
     * @param credential The updated credential object
     * @return True if successful, else false
     */
    bool updateCredential(const Credential& credential);
    /**
     * @brief Deletes a credential from the system's credential manager.
     * @param name The name of the credential to delete
     * @return True if successful, else false
     */
    bool deleteCredential(const std::string& name);
}

#endif //SYSTEMCREDENTIALS_H