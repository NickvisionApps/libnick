#include "database/sqlitevalue.h"

namespace Nickvision::Database
{
    SqliteValue::SqliteValue(sqlite3_value* value) noexcept
        : m_value{ sqlite3_value_dup(value) },
        m_type{ value ? sqlite3_value_type(value) : SQLITE_NULL }
    {

    }

    SqliteValue::SqliteValue(const SqliteValue& other) noexcept
        : m_value{ sqlite3_value_dup(other.m_value) },
        m_type{ other.m_type }
    {

    }

    SqliteValue::SqliteValue(SqliteValue&& other) noexcept
        : m_value{ other.m_value },
        m_type{ other.m_type }
    {
        other.m_value = nullptr;
        other.m_type = SQLITE_NULL;
    }


    SqliteValue::~SqliteValue() noexcept
    {
        if(m_value)
        {
            sqlite3_value_free(m_value);
        }
    }

    SqliteValue& SqliteValue::operator=(const SqliteValue& other) noexcept
    {
        if (this != &other)
        {
            if(m_value)
            {
                sqlite3_value_free(m_value);
            }
            m_value = sqlite3_value_dup(other.m_value);
            m_type = other.m_type;
        }
        return *this;
    }

    SqliteValue& SqliteValue::operator=(SqliteValue&& other) noexcept
    {
        if (this != &other)
        {
            if(m_value)
            {
                sqlite3_value_free(m_value);
            }
            m_value = other.m_value;
            m_type = other.m_type;
            other.m_value = nullptr;
            other.m_type = SQLITE_NULL;
        }
        return *this;
    }
}