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
 * A model of a keyring object for managing credentials.
 */

#ifndef KEYRING_H
#define KEYRING_H

#include <optional>
#include <string>
#include <vector>
#include "credential.h"
#include "store.h"

namespace Nickvision::Keyring
{
    /**
     * @brief A model of a keyring object for managing credentials.
     */
    class Keyring
    {
    public:
        /**
         * @brief Gets the name of the keyring.
         * @return The name of the keyring
         */
        const std::string& getName() const;
        /**
         * @brief Gets all credentials in the keyring.
         * @return The list of all credentials
         */
        std::vector<Credential> getAllCredentials() const;
        /**
         * @brief Gets the credential matching the provided id.
         * @param id The id of the credential
         * @return The credential matching the id, std::nullopt if no matching credential
         */
        std::optional<Credential> getCredential(int id) const;
        /**
         * @brief Gets the credentials containing the provided name.
         * @param name The name
         * @return The list of credentials matching the name
         */
        std::vector<Credential> getCredentials(const std::string& name) const;
        /**
         * @brief Adds a credential to the keyring.
         * @param credential The Credential to add
         * @return True if successful, else false
         */
        bool addCredential(const Credential& credential);
        /**
         * @brief Updates a credential in the keyring.
         * @param credential The Credential to update
         * @return True if successful, else false
         */
        bool updateCredential(const Credential& credential);
        /**
         * @brief Deletes a credential from the keyring.
         * @param id The id of the credential to delete
         * @return True if successful, else false
         */
        bool deleteCredential(int id);
        /**
         * @brief Destroys the keyring and all of its data from disk. Once this method is called, the object should no longer be referenced, with or without success.
         * @return True if successful, else false
         */
        bool destroy();

    private:
        /**
         * @brief Constructs a Keyring.
         * @param store The Store database for the Keyring
         */
        Keyring(const Store& store);
        Store m_store;

    public:
        /**
         * @brief Accesses a Keyring. The Keyring will first attempt to load an existing Store. If the Store doesn't exist, it will create a new Store.
         * @param name The name of the store
         * @param password The password to use for the store. If empty, the password will be fetched/created from the system's credential store
         * @return The newly accessed keyring, std::nullopt if accessing failed
         */
        static std::optional<Keyring> access(const std::string& name, std::string password = "");
        /**
         * @brief Gets whether or not a keyring exists with the provided name.
         * @param name The name of the keyring to check
         * @return True if a keyring with the provided name exists, else false
         */
        static bool exists(const std::string& name);
        /**
         * @brief Destroys a keyring and all of its data from disk.
         * @param name name of the keyring to destroy
         * @return True if successful, else false
         */
        static bool destroy(const std::string& name);
    };
}

#endif //KEYRING_H