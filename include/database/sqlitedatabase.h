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
 * A thread-safe sqlite (sqlcipher) database.
 */

#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include "sqlite.h"
#include "sqlitefunctioncontext.h"
#include "sqlitestatement.h"

namespace Nickvision::Database
{
    using SqliteCustomFunction = std::function<void(SqliteFunctionContext&)>;

    /**
     * @brief A thread-safe sqlite (sqlcipher) database. 
     */
    class SqliteDatabase
    {
    public:
        /**
         * @brief Constructs a SqliteDatabase.
         * @brief If the database if not encrypted, it will be unlocked automatically.
         * @param path The path to the database file
         * @param flags The flags for opening the database
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        SqliteDatabase(const std::filesystem::path& path, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
        SqliteDatabase(const SqliteDatabase&) = delete;
        /**
         * @brief Constructs a SqliteDatabase via move.
         * @param other The SqliteDatabase to move
         */
        SqliteDatabase(SqliteDatabase&& other) noexcept;
        /**
         * @brief Destructs a SqliteDatabase.
         */
        ~SqliteDatabase() noexcept;
        /**
         * @brief Gets the path of the database file.
         * @return The database file path
         */
        const std::filesystem::path& getPath() const noexcept;
        /**
         * @brief Gets whether or not the database is encrypted.
         * @return True if encrypted, else false 
         */
        bool isEncrypted() const noexcept;
        /**
         * @brief Gets whether or not the database is unlocked.
         * @return True if unlocked, else false
         */
        bool isUnlocked() const noexcept;
        /**
         * @brief Unlocks the database.
         * @brief If the database is not encrypted, this method will have no effect and return true.
         * @brief If the database is encrypted, this method must be called before any other operations.
         * @param password The password of the database
         * @return True if unlocked, else false
         */
        bool unlock(const std::string& password) noexcept;
        /**
         * @brief Sets the database's password.
         * @brief If the database is not encrypted, it will be encrypted with the password.
         * @brief If the database is encrypted and locked, this method will return false.
         * @brief If the database is encrypted and unlocked and the password is empty, it will be decrypted.
         * @brief If the database is encrypted and unlocked and the password is not empty, it will be reencrypted with the new password.
         * @param password The new database password
         * @return True if successful, else false
         * @throw std::runtime_error Thrown if database cannot be opened
         */
        bool setPassword(const std::string& password);
        /**
         * @brief Registers a custom sql function to the database.
         * @param name The name of the sql function
         * @param func The custom sql function
         * @param expectedArgs The number of args the sql function expects to receive (specify -1 for unlimited number of args)
         * @return True if function registered, else false
         */
        bool registerFunction(const std::string& name, const SqliteCustomFunction& func, int expectedArgs = -1) noexcept;
        /**
         * @brief Creates a new SqlStatement for the database.
         * @brief Executing the statement is controlled by calling step() on the statement object.
         * @param command The command to bind to the statement.
         * @return The new SqlStatement if successful
         * @return An empty SqlStatement if failed (can be checked with operator bool())
         * @throw std::runtime_error Thrown if error in executing sql statement
         */
        SqliteStatement createStatement(const std::string& command);
        /**
         * @brief Executes a sql command on the database.
         * @param command The command to execute
         * @return True if command returned SQLITE_OK, else false
         */
        bool execute(const std::string& command) noexcept;
        SqliteDatabase& operator=(const SqliteDatabase&) = delete;
        /**
         * @brief Assigns a SqliteDatabase via move.
         * @param other The SqliteDatabase to move
         * @return Reference to this SqliteDatabase
         */
        SqliteDatabase& operator=(SqliteDatabase&& other) noexcept;

    private:
        mutable std::mutex m_mutex;
        std::filesystem::path m_path;
        int m_flags;
        bool m_isEncrypted;
        bool m_isUnlocked;
        sqlite3* m_database;
        std::unordered_map<std::string, SqliteCustomFunction> m_customFunctions;
    };
}

#endif //SQLDATABASE_H