#include "database/sqlvalue.h"

namespace Nickvision::Database
{
    SqlValue::SqlValue(sqlite3_value* value)
        : m_value{ value }
    {

    }

    int SqlValue::getInt() const
    {
        return sqlite3_value_int(m_value);
    }

    sqlite3_int64 SqlValue::getInt64() const
    {
        return sqlite3_value_int64(m_value);
    }

    double SqlValue::getDouble() const
    {
        return sqlite3_value_double(m_value);
    }

    bool SqlValue::getBool() const
    {
        return static_cast<bool>(sqlite3_value_int(m_value));
    }

    char SqlValue::getChar() const
    {
        return static_cast<char>(sqlite3_value_int(m_value));
    }

    std::string SqlValue::getString() const
    {
        return { (const char*)sqlite3_value_text(m_value), static_cast<size_t>(sqlite3_value_bytes(m_value)) };
    }

    std::pair<const void*, size_t> SqlValue::getBlob() const
    {
        return { sqlite3_value_blob(m_value), static_cast<size_t>(sqlite3_value_bytes(m_value)) };
    }
}