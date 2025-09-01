#include "database/sqlitefunctioncontext.h"

namespace Nickvision::Database
{
    SqliteFunctionContext::SqliteFunctionContext(sqlite3_context* ctx, int argc, sqlite3_value** argv) noexcept
        : m_context{ ctx }
    {
        m_values.reserve(argc);
        for(int i = 0; i < argc; i++)
        {
            m_values.push_back({ argv[i] });
        }
    }

    SqliteFunctionContext::SqliteFunctionContext(SqliteFunctionContext&& other) noexcept
        : m_context{ other.m_context },
        m_values{ std::move(other.m_values) }
    {
        other.m_context = nullptr;
    }

    void* SqliteFunctionContext::getUserData() const noexcept
    {
        if(!m_context)
        {
            return nullptr;
        }
        return sqlite3_user_data(m_context);
    }

    const std::vector<SqliteValue>& SqliteFunctionContext::getArgs() const noexcept
    {
        return m_values;
    }

    void SqliteFunctionContext::result() noexcept
    {
        if(!m_context)
        {
            return;
        }
        sqlite3_result_null(m_context);
    }

    void SqliteFunctionContext::error(const std::string& err) noexcept
    {
        if(!m_context)
        {
            return;
        }
        sqlite3_result_error(m_context, err.c_str(), static_cast<int>(err.size()));
    }

    void SqliteFunctionContext::error(int err) noexcept
    {
        if(!m_context)
        {
            return;
        }
        sqlite3_result_error_code(m_context, err);
    }

    SqliteFunctionContext& SqliteFunctionContext::operator=(SqliteFunctionContext&& other) noexcept
    {
        if (this != &other)
        {
            m_context = other.m_context;
            m_values = std::move(other.m_values);
            other.m_context = nullptr;
        }
        return *this;
    }
}