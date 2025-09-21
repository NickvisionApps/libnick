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
 * A sqlite statement.
 */

#ifndef SQLITESTATEMENT_H
#define SQLITESTATEMENT_H

#include "sqlite.h"
#include "sqlitestepresult.h"

namespace Nickvision::Database
{
    /**
     * @brief A sqlite statement. 
     */
    class SqliteStatement
    {
    public:
        /**
         * @brief Constructs a SqliteStatement.
         * @param database The sqlite3 database
         * @param command The sql command to set to the statement
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        SqliteStatement(sqlite3* database, const std::string& command);
        SqliteStatement(const SqliteStatement&) = delete;
        /**
         * @brief Constructs a SqliteStatement via move.
         * @param other The other SqliteStatement to move
         */
        SqliteStatement(SqliteStatement&& other) noexcept;
        /**
         * @brief Destructs a SqliteStatement.
         */
        ~SqliteStatement() noexcept;
        /**
         * @brief Steps through the statement.
         * @returns SqliteStepResult
         */
        SqliteStepResult step() noexcept;
        /**
         * @brief Resets the statement to its initial state, clearing any bindings as well.
         */
        void reset() noexcept;
        /**
         * @brief Binds a value to a sqlite parameter.
         * @tparam T The type of the value to bind
         * @param index The index of the parameter
         * @param value The value to bind
         * @return True if bind was successful, else false
         */
        template<SupportedSqliteValue T>
        bool bind(int index, T value) noexcept
        {
            if(!m_statement)
            {
                return false;
            }
            if constexpr (std::is_same_v<T, int>)
            {
                return sqlite3_bind_int(m_statement, index, value) == SQLITE_OK;
            }
            else if constexpr (std::is_same_v<T, std::int64_t>)
            {
                return sqlite3_bind_int64(m_statement, index, value) == SQLITE_OK;
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return sqlite3_bind_double(m_statement, index, value) == SQLITE_OK;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return sqlite3_bind_int(m_statement, index, value ? 1 : 0) == SQLITE_OK;
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return sqlite3_bind_text(m_statement, index, value.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK;
            }
        }
        /**
         * @brief Gets the sqlite column value as a specific type.
         * @tparam T The type to get the sqlite column value as (Can be: int, std::int64_t, double, bool, std::string)
         * @return The sqlite column value as the specified type
         * @return A default value if the sqlite column value is not of the specified type
         */
        template<SupportedSqliteValue T>
        T getColumn(int index) noexcept
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if(!m_statement)
                {
                    return 0;
                }
                return sqlite3_column_int(m_statement, index);
            }
            else if constexpr (std::is_same_v<T, std::int64_t>)
            {
                if(!m_statement)
                {
                    return 0;
                }
                return sqlite3_column_int64(m_statement, index);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                if(!m_statement)
                {
                    return 0.0;
                }
                return sqlite3_column_double(m_statement, index);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if(!m_statement)
                {
                    return false;
                }
                return static_cast<bool>(sqlite3_column_int(m_statement, index));
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                if(!m_statement)
                {
                    return "";
                }
                return { reinterpret_cast<const char*>(sqlite3_column_text(m_statement, index)), static_cast<size_t>(sqlite3_column_bytes(m_statement, index)) };
            }
        }
        SqliteStatement& operator=(const SqliteStatement&) = delete;
        /**
         * @brief Assigns a SqliteStatement via move.
         * @param other The other SqliteStatement to move
         * @return Reference to this SqliteStatement
         */
        SqliteStatement& operator=(SqliteStatement&& other) noexcept;
        /**
         * @brief Gets whether or not the object is valid or not.
         * @return True if valid, else false 
         */
        operator bool() const noexcept;

    private:
        sqlite3_stmt* m_statement;
    };
}

#endif //SQLITESTATEMENT_H