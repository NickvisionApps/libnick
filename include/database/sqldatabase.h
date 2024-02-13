#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include "sqlite3.h"
#include "sqlstatement.h"

namespace Nickvision::Database
{
    /**
     * @brief An sqlite (sqlcipher) database. 
     */
    class SqlDatabase
    {
    public:
        /**
         * @brief Constructs a SqlDatabase.
         * @param path The path to the database file
         * @param flags The flags for opening the database
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        SqlDatabase(const std::filesystem::path& path, int flags);
        /**
         * @brief Copies a SqlDatabase object.
         * @param database The object to move
         */
        SqlDatabase(const SqlDatabase& database);
        /**
         * @brief Moves a SqlDatabase object.
         * @param database The object to move
         */
        SqlDatabase(SqlDatabase&& database) noexcept;
        /**
         * @brief Gets the path of the database file.
         * @return The database file path
         */
        const std::filesystem::path& getPath() const;
        /**
         * @brief Gets whether or not the database is encrypted.
         * @return True if encrypted, else false 
         */
        bool isEncrypted() const;
        /**
         * @brief Unlocks the database.
         * @brief If the database is not encrypted and is newly created, this will encrypt and set its password.
         * @param password The password of the database
         * @return True if unlocked, else false
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        bool unlock(const std::string& password);
        /**
         * @brief Changes the database's password.
         * @brief Passing an empty string will unencrypt the database.
         * @brief Changing the password on an encrypted database requires that the database be unlocked first, even if removing the password.
         * @param password The new database password
         * @return True if password changed, else false
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        bool changePassword(const std::string& password);
        /**
         * @brief Executes an sql command on the database.
         * @param command The command to execute
         * @return True if command returned SQLITE_OK, else false
         */
        bool exec(const std::string& command);
        /**
         * @brief Creates an new SqlStatement for the database.
         * @param command The command to bind to the statement.
         * @return The new SqlStatement
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        SqlStatement createStatement(const std::string& command);
        /**
         * @brief Copies a SqlDatabase object.
         * @param database The SqlDatabase to copy
         * @return this
         */
        SqlDatabase& operator=(const SqlDatabase& database);
        /**
         * @brief Moves a SqlDatabase object.
         * @param database The SqlDatabase to move
         * @return this
         */
        SqlDatabase& operator=(SqlDatabase&& database) noexcept;
        /**
         * @brief Gets whether or not the object is valid or not.
         * @return True if valid (if isEncrypted ? isUnlocked : true), else false 
         */
        operator bool() const;

    private:
        mutable std::mutex m_mutex;
        std::filesystem::path m_path;
        int m_flags;
        bool m_isEncrypted;
        std::shared_ptr<sqlite3> m_database;
        bool m_isUnlocked;
    };
}

#endif //SQLDATABASE_H