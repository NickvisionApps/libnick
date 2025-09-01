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
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include "credential.h"
#include "database/sqlitedatabase.h"

namespace Nickvision::Keyring
{
    /**
     * @brief A model of a keyring object for managing credentials.
     * @brief The keyring is an sqlite database, encrypted with a password stored in the system's credential manager.
     */
    class Keyring
    {
    public:
        /**
         * @brief Constructs a Keyring.
         * @brief If the system credential manager is not available, an in-memory database will be used (credentials will not be saved to disk).
         * @param name The name of the keyring
         * @throw std::runtime_error Thrown if error in database operations
         */
        Keyring(const std::string& name);
        Keyring(const Keyring&) = delete;
        /**
         * @brief Constructs a Keyring via move.
         * @param other The Keyring to move
         */
        Keyring(Keyring&& other) noexcept;
        /**
         * @brief Gets the name of the keyring.
         * @return The name of the keyring.
         */
        const std::string& getName() const noexcept;
        /**
         * @brief Gets whether the keyring is saving data to disk.
         * @return True if saving data to disk, else false
         */
        bool isSavingToDisk() const noexcept;
        /**
         * @brief Gets all credentials in the keyring.
         * @return The list of all credentials
         */
        const std::vector<Credential>& getAll() const noexcept;
        /**
         * @brief Gets the credential matching the provided name.
         * @param name The name of the credential
         * @return The credential matching the name, std::nullopt if no matching credential found
         */
        std::optional<Credential> get(const std::string& name) noexcept;
        /**
         * @brief Adds a credential to the keyring.
         * @param credential The credential to add
         * @return True if successful, else false
         */
        bool add(const Credential& credential) noexcept;
        /**
         * @brief Updates a credential in the keyring.
         * @param credential The credential to update
         * @return True if successful, else false
         */
        bool update(const Credential& credential) noexcept;
        /**
         * @brief Deletes a credential from the keyring.
         * @param name The name of the credential to delete
         * @return True if successful, else false
         */
        bool remove(const std::string& name) noexcept;
        /**
         * @brief Destroys the keyring.
         * @brief This will delete all data in the keyring and remove it from the system.
         * @brief The object should not be used after as all methods will return false.
         * @return True if successful, else false
         */
        bool destroy() noexcept;
        Keyring& operator=(const Keyring&) = delete;
        /**
         * @brief Assigns a Keyring via move.
         * @param other The Keyring to move
         */
        Keyring& operator=(Keyring&& other) noexcept;

    private:
        mutable std::mutex m_mutex;
        std::string m_name;
        std::unique_ptr<Database::SqliteDatabase> m_database;
        std::vector<Credential> m_credentials;
    };
}

#endif //KEYRING_H