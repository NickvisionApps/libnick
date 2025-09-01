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
 * A sqlite function context.
 */

#ifndef SQLITEFUNCTIONCONTEXT_H
#define SQLITEFUNCTIONCONTEXT_H

#include <vector>
#include "sqlite.h"
#include "sqlitevalue.h"

namespace Nickvision::Database
{
    /**
     * @brief A sqlite function context. 
     */
    class SqliteFunctionContext
    {
    public:
        /**
         * @brief Constructs an SqliteFunctionContext.
         * @param ctx sqlite3_context*
         * @param argc The number of arguments passed to the context
         * @param argv The array of sqlite3_value* passed to the context
         */
        SqliteFunctionContext(sqlite3_context* ctx, int argc, sqlite3_value** argv) noexcept;
        SqliteFunctionContext(const SqliteFunctionContext&) = delete;
        /**
         * @brief Constructs a SqliteFunctionContext via move.
         * @param other The other SqliteFunctionContext to move
         */
        SqliteFunctionContext(SqliteFunctionContext&& other) noexcept;
        /**
         * @brief Gets the pointer to the user data for the context.
         * @return The user data pointer
         */
        void* getUserData() const noexcept;
        /**
         * @brief Gets the list of SqliteValue arguments passed to the function.
         * @return The list of arguments
         */
        const std::vector<SqliteValue>& getArgs() const noexcept;
        /**
         * @brief Returns a NULL value from the sql function. 
         */
        void result() noexcept;
        /**
         * @brief Returns a value from the sqlite function.
         * @tparam T The type of the value to return
         * @param value The value to return
         */
        template<SupportedSqliteValue T>
        void result(const T& value) noexcept
        {
            if(!m_context)
            {
                return;
            }
            if constexpr (std::is_same_v<T, int>)
            {
                sqlite3_result_int(m_context, value);
            }
            else if constexpr (std::is_same_v<T, sqlite3_int64>)
            {
                sqlite3_result_int64(m_context, value);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                sqlite3_result_double(m_context, value);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                sqlite3_result_int(m_context, value ? 1 : 0);
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                sqlite3_result_text(m_context, value.c_str(), value.size(), SQLITE_TRANSIENT);
            }
        }
        /**
         * @brief Returns an error from the sql function.
         * @param err The error message 
         */
        void error(const std::string& err) noexcept;
        /**
         * @brief Returns an error from the sql function.
         * @param err The error code 
         */
        void error(int err) noexcept;
        SqliteFunctionContext& operator=(const SqliteFunctionContext&) = delete;
        /**
         * @brief Assigns a SqliteFunctionContext via move.
         * @param other The other SqliteFunctionContext to move
         * @return Reference to this SqliteFunctionContext
         */
        SqliteFunctionContext& operator=(SqliteFunctionContext&& other) noexcept;

    private:
        sqlite3_context* m_context;
        std::vector<SqliteValue> m_values;
    };
}

#endif //SQLITEFUNCTIONCONTEXT_H