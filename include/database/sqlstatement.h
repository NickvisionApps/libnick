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

#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <memory>
#include <string>
#include <utility>
#include <sqlcipher/sqlite3.h>

namespace Nickvision::Database
{
    /**
     * @brief A sqlite statement. 
     */
    class SqlStatement
    {
    public:
        /**
         * @brief Constructs a SqlStatement.
         * @param database The sqlite3 database
         * @param command The sql command to bind to the statement
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        SqlStatement(const std::shared_ptr<sqlite3>& database, const std::string& command);
        /**
         * @brief Binds an int to a parameter.
         * @param index The index of the parameter
         * @param value The int to bind
         * @return True if bind was successful, else false
         */
        bool bind(int index, int value);
        /**
         * @brief Binds an int64 to a parameter.
         * @param index The index of the parameter
         * @param value The int64 to bind
         * @return True if bind was successful, else false
         */
        bool bind(int index, sqlite3_int64 value);
        /**
         * @brief Binds a double to a parameter.
         * @param index The index of the parameter
         * @param value The double to bind
         * @return True if bind was successful, else false
         */
        bool bind(int index, double value);
        /**
         * @brief Binds a bool to a parameter.
         * @param index The index of the parameter
         * @param value The bool to bind
         * @return True if bind was successful, else false
         */
        bool bind(int index, bool value);
        /**
         * @brief Binds a string to a parameter.
         * @param index The index of the parameter
         * @param value The string to bind
         * @return True if bind was successful, else false 
         */
        bool bind(int index, const std::string& value);
        /**
         * @brief Binds a blob to a parameter.
         * @param index The index of the parameter
         * @param value The blob to bind
         * @param n The size of the blob
         * @return True if bind was successful, else false
         */
        bool bind(int index, void* value, int n);
        /**
         * @brief Steps through the statement.
         * @returns True if SQLITE_ROW, else false
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        bool step();
        /**
         * @brief Gets an int from a column.
         * @param index The index of the column
         * @return The int from the column 
         */
        int getColumnInt(int index) const;
        /**
         * @brief Gets an int64 from a column.
         * @param index The index of the column
         * @return The int64 from the column 
         */
        sqlite3_int64 getColumnInt64(int index) const;
        /**
         * @brief Gets a double from a column.
         * @param index The index of the column
         * @return The double from the column 
         */
        double getColumnDouble(int index) const;
        /**
         * @brief Gets a bool from a column.
         * @param index The index of the column
         * @return The bool from the column 
         */
        bool getColumnBool(int index) const;
        /**
         * @brief Gets a string from a column.
         * @param index The index of the column
         * @return The string from the column 
         */
        std::string getColumnString(int index) const;
        /**
         * @brief Gets a blob from a column.
         * @param index The index of the column
         * @return A pair of the blob from the column and the blob's size
         */
        std::pair<const void*, size_t> getColumnBlob(int index) const;
        /**
         * @brief Gets whether or not the object is valid or not.
         * @return True if valid, else false 
         */
        operator bool() const;

    private:
        std::shared_ptr<sqlite3_stmt> m_statement;
    };
}

#endif //SQLSTATEMENT_H