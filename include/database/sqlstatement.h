#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <memory>
#include <string>
#include <utility>
#include "sqlite3.h"

namespace Nickvision::Database
{
    /**
     * @brief An sqlite statement. 
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
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, int value);
        /**
         * @brief Binds an int64 to a parameter.
         * @param index The index of the parameter
         * @param value The int64 to bind
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, sqlite3_int64 value);
        /**
         * @brief Binds a double to a parameter.
         * @param index The index of the parameter
         * @param value The double to bind
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, double value);
        /**
         * @brief Binds a bool to a parameter.
         * @param index The index of the parameter
         * @param value The bool to bind
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, bool value);
        /**
         * @brief Binds a string to a parameter.
         * @param index The index of the parameter
         * @param value The string to bind
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, const std::string& value);
        /**
         * @brief Binds a blob to a parameter.
         * @param index The index of the parameter
         * @param value The blob to bind
         * @param n The size of the blob
         * @throw std::runtime_error Thrown if error in executing sql statement 
         */
        void bind(int index, void* value, int n);
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
        int getColumnInt(int index);
        /**
         * @brief Gets an int64 from a column.
         * @param index The index of the column
         * @return The int64 from the column 
         */
        sqlite3_int64 getColumnInt64(int index);
        /**
         * @brief Gets a double from a column.
         * @param index The index of the column
         * @return The double from the column 
         */
        double getColumnDouble(int index);
        /**
         * @brief Gets a bool from a column.
         * @param index The index of the column
         * @return The bool from the column 
         */
        bool getColumnBool(int index);
        /**
         * @brief Gets a string from a column.
         * @param index The index of the column
         * @return The string from the column 
         */
        std::string getColumnString(int index);
        /**
         * @brief Gets a blob from a column.
         * @param index The index of the column
         * @return A pair of the blob from the column and the blob's size
         */
        std::pair<const void*, int> getColumnBlob(int index);
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