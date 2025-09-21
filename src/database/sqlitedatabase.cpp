#include "database/sqlitedatabase.h"
#include <stdexcept>

namespace Nickvision::Database
{
    SqliteDatabase::SqliteDatabase(const std::filesystem::path& path, int flags)
        : m_path{ path },
        m_flags{ flags },
        m_isEncrypted{ false },
        m_isUnlocked{ true },
        m_database{ nullptr }
    {
        if (sqlite3_open_v2(m_path.string().c_str(), &m_database, m_flags, nullptr) != SQLITE_OK)
        {
            throw std::runtime_error("Unable to open sql database.");
        }
        if(sqlite3_exec(m_database, "SELECT count(*) FROM sqlite_master;", nullptr, nullptr, nullptr) != SQLITE_OK)
        {
            m_isEncrypted = true;
            m_isUnlocked = false;
        }
    }

    SqliteDatabase::SqliteDatabase(SqliteDatabase&& other) noexcept
    {
        std::lock_guard<std::mutex> lock{ other.m_mutex };
        m_path = std::move(other.m_path);
        m_isEncrypted = std::move(other.m_isEncrypted);
        m_isUnlocked = std::move(other.m_isUnlocked);
        m_database = other.m_database;
        other.m_database = nullptr;
        m_customFunctions = std::move(other.m_customFunctions);
    }

    SqliteDatabase::~SqliteDatabase() noexcept
    {
        if (m_database)
        {
            sqlite3_close(m_database);
        }
    }

    const std::filesystem::path& SqliteDatabase::getPath() const noexcept
    {
        return m_path;
    }

    bool SqliteDatabase::isEncrypted() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_database)
        {
            return false;
        }
        return m_isEncrypted;
    }

    bool SqliteDatabase::isUnlocked() const noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_database)
        {
            return false;
        }
        return m_isUnlocked;
    }

    bool SqliteDatabase::unlock(const std::string& password) noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_database)
        {
            return false;
        }
        if(!m_isEncrypted || m_isUnlocked)
        {
            return true;
        }
        if(sqlite3_key(m_database, password.c_str(), static_cast<int>(password.size())) != SQLITE_OK)
        {
            m_isUnlocked = false;
        }
        else
        {
            m_isUnlocked = sqlite3_exec(m_database, "SELECT count(*) FROM sqlite_master;", nullptr, nullptr, nullptr) == SQLITE_OK;
        }
        return m_isUnlocked;
    }

    bool SqliteDatabase::setPassword(const std::string& password)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_database)
        {
            return false;
        }
        //Encrypt non encrypted database
        if(!m_isEncrypted)
        {
            if(password.empty())
            {
                return true;
            }
            //If empty database, can use sqlite3_key
            SqliteStatement statement{ m_database, "SELECT count(*) FROM sqlite_master;" };
            int tableCount{ -1 };
            if(statement.step() == SqliteStepResult::Row)
            {
                tableCount = statement.getColumn<int>(0);
            }
            if(tableCount == 0)
            {
                if(sqlite3_key(m_database, password.c_str(), static_cast<int>(password.size())) != SQLITE_OK)
                {
                    return false;
                }
                else
                {
                    m_isUnlocked = sqlite3_exec(m_database, "SELECT count(*) FROM sqlite_master;", nullptr, nullptr, nullptr) == SQLITE_OK;
                    m_isEncrypted = true;
                    return true;
                }
            }
            //Create temp encrypted database
            std::filesystem::path tempPath{ (m_path.string() + ".encrypt") };
            std::string cmd{ "ATTACH DATABASE '" + tempPath.string() + "' AS encrypted KEY '" + password + "'" };
            sqlite3_exec(m_database, cmd.c_str(), nullptr, nullptr, nullptr);
            sqlite3_exec(m_database, "SELECT sqlcipher_export('encrypted')", nullptr, nullptr, nullptr);
            sqlite3_exec(m_database, "DETACH DATABASE encrypted", nullptr, nullptr, nullptr);
            //Remove old encrypted database
            sqlite3_close(m_database);
            std::filesystem::remove(m_path);
            std::filesystem::rename(tempPath, m_path);
            //Open new encrypted database
            if (sqlite3_open_v2(m_path.string().c_str(), &m_database, m_flags, nullptr) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to open sql database.");
            }
            if(sqlite3_key(m_database, password.c_str(), static_cast<int>(password.size())) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to open sql database with password.");
            }
            m_isUnlocked = sqlite3_exec(m_database, "SELECT count(*) FROM sqlite_master;", nullptr, nullptr, nullptr) == SQLITE_OK;
            m_isEncrypted = true;
            return true;
        }
        //Encrypted database locked
        if(!m_isUnlocked)
        {
            return false;
        }
        //Remove encryption
        if(password.empty())
        {
            //Create temporary decrypted database
            std::filesystem::path tempPath{ (m_path.string() + ".decrypt") };
            std::string cmd{ "ATTACH DATABASE '" + tempPath.string() + "' AS plaintext KEY ''" };
            sqlite3_exec(m_database, cmd.c_str(), nullptr, nullptr, nullptr);
            sqlite3_exec(m_database, "SELECT sqlcipher_export('plaintext')", nullptr, nullptr, nullptr);
            sqlite3_exec(m_database, "DETACH DATABASE plaintext", nullptr, nullptr, nullptr);
            //Remove old encrypted database
            sqlite3_close(m_database);
            std::filesystem::remove(m_path);
            std::filesystem::rename(tempPath, m_path);
            //Open new decrypted database
            if (sqlite3_open_v2(m_path.string().c_str(), &m_database, m_flags, nullptr) != SQLITE_OK)
            {
                throw std::runtime_error("Unable to open sql database.");
            }
            m_isEncrypted = false;
            m_isUnlocked = true;
            return true;
        }
        //Reencrypt with new password
        sqlite3_rekey(m_database, password.c_str(), static_cast<int>(password.size()));
        return true;
    }

    bool SqliteDatabase::registerFunction(const std::string& name, const SqliteCustomFunction& func, int expectedArgs) noexcept
    {
        m_customFunctions[name] = func;
        return sqlite3_create_function(m_database, name.c_str(), expectedArgs, SQLITE_UTF8, &m_customFunctions[name], +[](sqlite3_context* ctx, int argc, sqlite3_value** argv)
        {
            SqliteFunctionContext context{ ctx, argc, argv };
            SqliteCustomFunction& func{ *(static_cast<SqliteCustomFunction*>(context.getUserData())) };
            func(context);
        }, nullptr, nullptr) == SQLITE_OK;
    }

    SqliteStatement SqliteDatabase::createStatement(const std::string& command)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (!m_database || !m_isUnlocked)
        {
            return { nullptr, "" };
        }
        return { m_database, command };
    }

    bool SqliteDatabase::execute(const std::string& command) noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (!m_database || !m_isUnlocked)
        {
            return false;
        }
        return sqlite3_exec(m_database, command.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    SqliteDatabase& SqliteDatabase::operator=(SqliteDatabase&& other) noexcept
    {
        if (this != &other)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ other.m_mutex };
            m_path = std::move(other.m_path);
            m_isEncrypted = std::move(other.m_isEncrypted);
            m_isUnlocked = std::move(other.m_isUnlocked);
            m_database = other.m_database;
            other.m_database = nullptr;
            m_customFunctions = std::move(other.m_customFunctions);
        }
        return *this;
    }
}
