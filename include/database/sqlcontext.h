#ifndef SQLCONTEXT_H
#define SQLCONTEXT_H

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC
#endif

#include <string>
#include <vector>
#include "sqlite3.h"
#include "sqlvalue.h"

namespace Nickvision::Database
{
    /**
     * @brief A sqlite function context. 
     */
    class SqlContext
    {
    public:
        /**
         * @brief Constructs an SqlContext.
         * @param ctx sqlite3_context*
         */
        SqlContext(sqlite3_context* ctx, int argc, sqlite3_value** argv);
        /**
         * @brief Gets the pointer to the user data for the context.
         * @return The user data pointer
         */
        void* getUserData() const;
        /**
         * @brief Gets the list of SqlValue arguments passed to the function.
         * @return The list of arguments
         */
        const std::vector<SqlValue>& getArgs() const;
        /**
         * @brief Returns a NULL value from the sql function. 
         */
        void result();
        /**
         * @brief Returns an int value from the sql function. 
         * @param value The int value to return
         */
        void result(int value);
        /**
         * @brief Returns an int64 value from the sql function. 
         * @param value The int64 value to return
         */
        void result(sqlite3_int64 value);
        /**
         * @brief Returns a double value from the sql function. 
         * @param value The double value to return
         */
        void result(double value);
        /**
         * @brief Returns a bool value from the sql function. 
         * @param value The bool value to return
         */
        void result(bool value);
        /**
         * @brief Returns a char value from the sql function.
         * @param value The char value to return 
         */
        void result(char value);
        /**
         * @brief Returns a string value from the sql function. 
         * @param value The string value to return
         */
        void result(const std::string& value);
         /**
         * @brief Returns a blob value from the sql function. 
         * @param value The blob value to return
         * @param n The size of the blob value
         */
        void result(void* value, int n);
        /**
         * @brief Returns an error from the sql function.
         * @param err The error message 
         */
        void error(const std::string& err);
        /**
         * @brief Returns an error from the sql function.
         * @param err The error code 
         */
        void error(int err);

    private:
        sqlite3_context* m_context;
        std::vector<SqlValue> m_values;
    };
}

#endif //SQLCONTEXT_H