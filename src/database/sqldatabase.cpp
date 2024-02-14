#include "database/sqldatabase.h"
#include <stdexcept>

namespace Nickvision::Database
{
    SqlDatabase::SqlDatabase(const std::filesystem::path& path, int flags)
        : m_path{ path },
        m_flags{ flags },
        m_isEncrypted{ false },
        m_database{ nullptr },
        m_isUnlocked{ true }
    {
        sqlite3* database{ nullptr };
        if (sqlite3_open_v2(m_path.string().c_str(), &database, m_flags, nullptr) != SQLITE_OK)
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

    sqlite3* SqlDatabase::c_obj()
    {
        return m_database.get();
    }

    bool SqlDatabase::unlock(const std::string& password)
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
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
            lock.unlock();
            m_isUnlocked = exec("PRAGMA schema_version");
        }
        return m_isUnlocked;
    }

    bool SqlDatabase::changePassword(const std::string& password)
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        //Remove password if empty (decrypt)
        if(password.empty())
        {
            if(m_isEncrypted)
            {
                if(!m_isUnlocked)
                {
                    return false;
                }
                //Create temp decrypted database
                std::filesystem::path tempPath{ (m_path.string() + ".decrypt") };
                lock.unlock();
                exec("ATTACH DATABASE '" + tempPath.string() + "' AS plaintext KEY ''");
                exec("SELECT sqlcipher_export('plaintext')");
                exec("DETACH DATABASE plaintext");
                lock.lock();
                //Remove old encrypted database
                m_database.reset();
                m_database = nullptr;
                std::filesystem::remove(m_path);
                std::filesystem::rename(tempPath, m_path);
                //Open new decrypted database
                sqlite3* database{ nullptr };
                if (sqlite3_open_v2(m_path.string().c_str(), &database, m_flags, nullptr) != SQLITE_OK)
                {
                    throw std::runtime_error("Unable to open sql database.");
                }
                m_database = { database, [](sqlite3* sql)
                {
                    sqlite3_close(sql);
                } };
                m_isEncrypted = false;
                m_isUnlocked = true;
            }
        }
        //Change password on encrypted database
        else if(m_isEncrypted)
        {
            if(!m_isUnlocked)
            {
                return false;
            }
            sqlite3_rekey(m_database.get(), password.c_str(), static_cast<int>(password.size()));
        }
        //Set new password (encrypts for the first time)
        else
        {
            //Create temp encrypted database
            std::filesystem::path tempPath{ (m_path.string() + ".encrypt") };
            lock.unlock();
            exec("ATTACH DATABASE '" + tempPath.string() + "' AS encrypted  KEY '" + password + "'");
            exec("SELECT sqlcipher_export('encrypted')");
            exec("DETACH DATABASE encrypted");
            lock.lock();
            //Remove old encrypted database
            m_database.reset();
            m_database = nullptr;
            std::filesystem::remove(m_path);
            std::filesystem::rename(tempPath, m_path);
            //Open new encrypted database
            sqlite3* database{ nullptr };
            if (sqlite3_open_v2(m_path.string().c_str(), &database, m_flags, nullptr) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to open sql database.");
            }
            if(sqlite3_key(m_database.get(), password.c_str(), static_cast<int>(password.size())) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to key sql database.");
            }
            m_database = { database, [](sqlite3* sql)
            {
                sqlite3_close(sql);
            } };
            m_isEncrypted = false;
            m_isUnlocked = true;
        }
        return true;
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

    void SqlDatabase::registerFunction(const std::string& name, const SqliteCustomFunction& func, int expectedArgs)
    {
        m_custom[name] = func;
        sqlite3_create_function(m_database.get(), name.c_str(), expectedArgs, SQLITE_UTF8, &m_custom[name], +[](sqlite3_context* ctx, int argc, sqlite3_value** argv)
        {
            SqlContext context{ ctx, argc, argv };
            SqliteCustomFunction& func{ *(static_cast<SqliteCustomFunction*>(context.getUserData())) };
            func(context);
        }, nullptr, nullptr);
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