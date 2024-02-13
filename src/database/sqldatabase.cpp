#include "database/sqldatabase.h"
#include <stdexcept>

namespace Nickvision::Database
{
    SqlDatabase::SqlDatabase(const std::filesystem::path& path, int flags)
        : m_path{ path },
        m_isEncrypted{ false },
        m_database{ nullptr },
        m_isUnlocked{ true }
    {
        sqlite3* database{ nullptr };
        if (sqlite3_open_v2(m_path.string().c_str(), &database, flags, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error("Unable to open sql database.");
        }
        if(sqlite3_exec(database, "PRAGMA schema_version", nullptr, nullptr, nullptr) != SQLITE_OK)
        {
            m_isEncrypted = true;
            m_isUnlocked = false;
        }
        m_database = { database, [](sqlite3* sql)
        {
            sqlite3_close(sql);
        } };
    }

    SqlDatabase::SqlDatabase(const SqlDatabase& database)
    {
        std::lock_guard<std::mutex> lock{ database.m_mutex };
        m_path = database.m_path;
        m_isEncrypted = database.m_isEncrypted;
        m_database = database.m_database;
        m_isUnlocked = database.m_isUnlocked;
    }

    SqlDatabase::SqlDatabase(SqlDatabase&& database) noexcept
    {
        std::lock_guard<std::mutex> lock{ database.m_mutex };
        m_path = std::move(database.m_path);
        m_isEncrypted = std::move(database.m_isEncrypted);
        m_database = std::move(database.m_database);
        m_isUnlocked = std::move(database.m_isUnlocked);
    }

    const std::filesystem::path& SqlDatabase::getPath() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_path;
    }

    bool SqlDatabase::isEncrypted() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_isEncrypted;
    }

    bool SqlDatabase::unlock(const std::string& password)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_isEncrypted)
        {
            m_isUnlocked = true;
        }
        else
        {
            if(sqlite3_key(m_database.get(), password.c_str(), static_cast<int>(password.size())) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to key sql database.");
            }
            m_isUnlocked = exec("PRAGMA schema_version");
        }
        return m_isUnlocked;
    }

    bool SqlDatabase::changePassword(const std::string& password)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
    }

    bool SqlDatabase::exec(const std::string& command)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return sqlite3_exec(m_database.get(), command.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    SqlStatement SqlDatabase::createStatement(const std::string& command)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return { m_database, command };
    }

    SqlDatabase& SqlDatabase::operator=(const SqlDatabase& database)
    {
        if (this != &database)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ database.m_mutex };
            m_path = database.m_path;
            m_isEncrypted = database.m_isEncrypted;
            m_database = database.m_database;
            m_isUnlocked = database.m_isUnlocked;
        }
        return *this;
    }

    SqlDatabase& SqlDatabase::operator=(SqlDatabase&& database) noexcept
    {
        if (this != &database)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ database.m_mutex };
            m_path = std::move(database.m_path);
            m_isEncrypted = std::move(database.m_isEncrypted);
            m_database = std::move(database.m_database);
            m_isUnlocked = std::move(database.m_isUnlocked);
        }
        return *this;
    }

    SqlDatabase::operator bool() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_database.operator bool() && (m_isEncrypted ? m_isUnlocked : true);
    }
}