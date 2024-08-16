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

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "credential.h"
#include "database/sqldatabase.h"

namespace Nickvision::Keyring
{
    /**
     * @brief A model of a keyring object for managing credentials.
     * @brief The keyring is encrypted with a password stored in the system's credential manager.
     */
    class Keyring
    {
    public:
        /**
         * @brief Constructs a Keyring.
         * @brief If the system credential manager is not available, the object will be functional but will not save any data to disk.
         * @param name The name of the keyring
         */
        Keyring(const std::string& name);
        /**
         * @brief Gets the name of the keyring.
         * @return The name of the keyring.
         */
        const std::string& getName() const;
        /**
         * @brief Gets whether the keyring is saving data to disk.
         * @return True if saving data to disk, else false
         */
        bool isSavingToDisk() const;
        /**
         * @brief Gets all credentials in the keyring.
         * @return The list of all credentials
         */
        const std::vector<Credential>& getCredentials() const;
        /**
         * @brief Gets the credential matching the provided name.
         * @param name The name of the credential
         * @return The credential matching the name, std::nullopt if no matching credential found
         */
        std::optional<Credential> getCredential(const std::string& name);
        /**
         * @brief Adds a credential to the keyring.
         * @param credential The credential to add
         * @return True if successful, else false
         */
        bool addCredential(const Credential& credential);
        /**
         * @brief Updates a credential in the keyring.
         * @param credential The credential to update
         * @return True if successful, else false
         */
        bool updateCredential(const Credential& credential);
        /**
         * @brief Deletes a credential from the keyring.
         * @param name The name of the credential to delete
         * @return True if successful, else false
         */
        bool deleteCredential(const std::string& name);
        /**
         * @brief Destroys the keyring.
         * @brief This will delete all data in the keyring and put the object in a state where no data can be saved to disk.
         * @return True if successful, else false
         */
        bool destroy();

    private:
        std::string m_name;
        std::shared_ptr<Database::SqlDatabase> m_database;
        std::vector<Credential> m_credentials;
    };
}

#endif //KEYRING_H