#include "database/sqlstatement.h"
#include <stdexcept>

namespace Nickvision::Database
{
    SqlStatement::SqlStatement(const std::shared_ptr<sqlite3>& database, const std::string& command)
    {
        sqlite3_stmt* statement;
        if(sqlite3_prepare_v2(database.get(), command.c_str(), static_cast<int>(command.size()), &statement, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error("Unable to create sql statement.");
        }
        m_statement = { statement, [](sqlite3_stmt* stmt)
        {
            sqlite3_finalize(stmt);
        } };
    }

    bool SqlStatement::bind(int index, int value)
    {
        return sqlite3_bind_int(m_statement.get(), index, value) == SQLITE_OK;
    }

    bool SqlStatement::bind(int index, sqlite3_int64 value)
    {
        return sqlite3_bind_int64(m_statement.get(), index, value) == SQLITE_OK;
    }

    bool SqlStatement::bind(int index, double value)
    {
        return sqlite3_bind_double(m_statement.get(), index, value) == SQLITE_OK;
    }

    bool SqlStatement::bind(int index, bool value)
    {
        return sqlite3_bind_int(m_statement.get(), index, static_cast<int>(value)) == SQLITE_OK;
    }

    bool SqlStatement::bind(int index, const std::string& value)
    {
        return sqlite3_bind_text(m_statement.get(), index, value.c_str(), static_cast<int>(value.size()), SQLITE_TRANSIENT) == SQLITE_OK;
    }

    bool SqlStatement::bind(int index, void* value, int n)
    {
        return sqlite3_bind_blob(m_statement.get(), index, value, n, SQLITE_TRANSIENT) == SQLITE_OK;
    }

    bool SqlStatement::step()
    {
        int res{ sqlite3_step(m_statement.get()) };
        if(res != SQLITE_ROW && res != SQLITE_DONE)
        {
            throw std::runtime_error("Unable to step sql statement.");
        }
        return res == SQLITE_ROW;
    }

    int SqlStatement::getColumnInt(int index) const
    {
        return sqlite3_column_int(m_statement.get(), index);
    }

    sqlite3_int64 SqlStatement::getColumnInt64(int index) const
    {
        return sqlite3_column_int64(m_statement.get(), index);
    }

    double SqlStatement::getColumnDouble(int index) const
    {
        return sqlite3_column_double(m_statement.get(), index);
    }
    
    bool SqlStatement::getColumnBool(int index) const
    {
        return static_cast<bool>(sqlite3_column_int(m_statement.get(), index));
    }

    std::string SqlStatement::getColumnString(int index) const
    {
        return { (const char*)sqlite3_column_text(m_statement.get(), index), static_cast<size_t>(sqlite3_column_bytes(m_statement.get(), index)) };
    }

    std::pair<const void*, size_t> SqlStatement::getColumnBlob(int index) const
    {
        return { sqlite3_column_blob(m_statement.get(), index), static_cast<size_t>(sqlite3_column_bytes(m_statement.get(), index)) };
    }

    SqlStatement::operator bool() const
    {
        return m_statement.operator bool();
    }
}