#include "database/sqlitestatement.h"
#include <stdexcept>

namespace Nickvision::Database
{
    SqliteStatement::SqliteStatement(sqlite3* database, const std::string& command)
        : m_statement{ nullptr }
    {
        if(sqlite3_prepare_v2(database, command.c_str(), static_cast<int>(command.size()), &m_statement, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error("Unable to create sql statement.");
        }
    }

    SqliteStatement::SqliteStatement(SqliteStatement&& other) noexcept
        : m_statement{ other.m_statement }
    {
        other.m_statement = nullptr;
    }

    SqliteStatement::~SqliteStatement() noexcept
    {
        if(m_statement)
        {
            sqlite3_finalize(m_statement);
        }
    }

    SqliteStepResult SqliteStatement::step() noexcept
    {
        if(!m_statement)
        {
            return SqliteStepResult::Error;
        }
        int res{ sqlite3_step(m_statement) };
        if(res == SQLITE_ROW)
        {
            return SqliteStepResult::Row;
        }
        else if(res == SQLITE_DONE)
        {
            return SqliteStepResult::Done;
        }
        return SqliteStepResult::Error;
    }

    void SqliteStatement::reset() noexcept
    {
        if(m_statement)
        {
            sqlite3_reset(m_statement);
            sqlite3_clear_bindings(m_statement);
        }
    }

    SqliteStatement& SqliteStatement::operator=(SqliteStatement&& other) noexcept
    {
        if(this != &other)
        {
            if(m_statement)
            {
                sqlite3_finalize(m_statement);
            }
            m_statement = other.m_statement;
            other.m_statement = nullptr;
        }
        return *this;
    }

    SqliteStatement::operator bool() const noexcept
    {
        return m_statement != nullptr;
    }
}