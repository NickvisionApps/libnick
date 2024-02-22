#include "database/sqlcontext.h"

namespace Nickvision::Database
{
    SqlContext::SqlContext(sqlite3_context* ctx, int argc, sqlite3_value** argv)
        : m_context{ ctx }
    {
        m_values.reserve(argc);
        for(int i = 0; i < argc; i++)
        {
            m_values.push_back({ argv[i] });
        }
    }

    void* SqlContext::getUserData() const
    {
        return sqlite3_user_data(m_context);
    }

    const std::vector<SqlValue>& SqlContext::getArgs() const
    {
        return m_values;
    }

    void SqlContext::result()
    {
        sqlite3_result_null(m_context);
    }

    void SqlContext::result(int value)
    {
        sqlite3_result_int(m_context, value);
    }

    void SqlContext::result(sqlite_int64 value)
    {
        sqlite3_result_int64(m_context, value);
    }

    void SqlContext::result(double value)
    {
        sqlite3_result_double(m_context, value);
    }

    void SqlContext::result(bool value)
    {
        sqlite3_result_int(m_context, static_cast<int>(value));
    }

    void SqlContext::result(char value)
    {
        sqlite3_result_int(m_context, static_cast<int>(value));
    }

    void SqlContext::result(const std::string& value)
    {
        sqlite3_result_text(m_context, value.c_str(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
    }

    void SqlContext::result(void* value, int n)
    {
        sqlite3_result_blob(m_context, value, n, SQLITE_TRANSIENT);
    }

    void SqlContext::error(const std::string& err)
    {
        sqlite3_result_error(m_context, err.c_str(), static_cast<int>(err.size()));
    }

    void SqlContext::error(int err)
    {
        sqlite3_result_error_code(m_context, err);
    }
}