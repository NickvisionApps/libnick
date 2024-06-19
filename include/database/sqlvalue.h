#ifndef SQLVALUE_H
#define SQLVALUE_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <string>
#include <utility>
#ifdef _WIN32
#include <sqlcipher/sqlite3.h>
#else
#include "sqlite3.h"
#endif

namespace Nickvision::Database
{
    /**
     * @brief A sqlite value. 
     */
    class SqlValue
    {
    public:
        /**
         * @brief Constructs an SqlValue.
         * @param value sqlite3_value*
         */
        SqlValue(sqlite3_value* value);
        /**
         * @brief Gets an int from a value.
         * @return The int from the value 
         */
        int getInt() const;
        /**
         * @brief Gets an int64 from a value.
         * @return The int64 from the value 
         */
        sqlite3_int64 getInt64() const;
        /**
         * @brief Gets a double from a value.
         * @return The double from the value 
         */
        double getDouble() const;
        /**
         * @brief Gets a bool from a value.
         * @return The bool from the value 
         */
        bool getBool() const;
        /**
         * @brief Gets a string from a value.
         * @return The string from the value 
         */
        std::string getString() const;
        /**
         * @brief Gets a blob from a value.
         * @return A pair of the blob from the value and the blob's size
         */
        std::pair<const void*, size_t> getBlob() const;

    private:
        sqlite3_value* m_value;
    };
}

#endif //SQLVALUE_H

