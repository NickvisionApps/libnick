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
 * A sqlite value.
 */

#ifndef SQLITEVALUE_H
#define SQLITEVALUE_H

#include "sqlite.h"

namespace Nickvision::Database
{
    /**
     * @brief A sqlite value. 
     */
    class SqliteValue
    {
    public:
        /**
         * @brief Constructs an SqliteValue.
         * @param value sqlite3_value*
         */
        SqliteValue(sqlite3_value* value) noexcept;
        /**
         * @brief Constructs a SqliteValue via copy.
         * @param other The other SqliteValue to copy
         */
        SqliteValue(const SqliteValue& other) noexcept;
        /**
         * @brief Constructs a SqliteValue via move.
         * @param other The other SqliteValue to move
         */
        SqliteValue(SqliteValue&& other) noexcept;
        /**
         * @brief Destructs the SqliteValue.
         */
        ~SqliteValue() noexcept;
        /**
         * @brief Gets the sqlite value as a specific type.
         * @tparam T The type to get the sqlite value as (Can be: int, std::int64_t, double, bool, std::string)
         * @return The sqlite value as the specified type
         * @return A default value if the sqlite value is not of the specified type
         */
        template<SupportedSqliteValue T>
        T as() const noexcept
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if(!m_value || m_type != SQLITE_INTEGER)
                {
                    return 0;
                }
                return sqlite3_value_int(m_value);
            }
            else if constexpr (std::is_same_v<T, std::int64_t>)
            {
                if(!m_value || m_type != SQLITE_INTEGER)
                {
                    return 0;
                }
                return sqlite3_value_int64(m_value);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                if(!m_value || m_type != SQLITE_FLOAT)
                {
                    return 0.0;
                }
                return sqlite3_value_double(m_value);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if(!m_value || m_type != SQLITE_INTEGER)
                {
                    return false;
                }
                return static_cast<bool>(sqlite3_value_int(m_value));
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                if(!m_value || m_type != SQLITE3_TEXT)
                {
                    return "";
                }
                return { reinterpret_cast<const char*>(sqlite3_value_text(m_value)), static_cast<size_t>(sqlite3_value_bytes(m_value)) };
            }
        }
        /**
         * @brief Assigns a SqliteValue via copy.
         * @param other The other SqliteValue to copy
         */
        SqliteValue& operator=(const SqliteValue& other) noexcept;
        /**
         * @brief Assigns a SqliteValue via move.
         * @param other The other SqliteValue to move
         */
        SqliteValue& operator=(SqliteValue&& other) noexcept;

    private:
        sqlite3_value* m_value;
        int m_type;
    };
}

#endif //SQLITEVALUE_H

