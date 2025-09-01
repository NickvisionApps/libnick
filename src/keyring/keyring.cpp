#include "keyring/keyring.h"
#include "database/sqlitestatement.h"
#include "filesystem/userdirectories.h"
#include "system/credentials.h"

using namespace Nickvision::Database;
using namespace Nickvision::Filesystem;
using namespace Nickvision::System;

namespace Nickvision::Keyring
{
    Keyring::Keyring(const std::string& name)
        : m_name{ name },
        m_database{ nullptr }
    {
        std::optional<Credential> credential{ Credentials::get(name) };
        if(!credential)
        {
            credential = Credentials::create(name);
        }
        if(credential)
        {
            static std::filesystem::path keyringDir{ UserDirectories::get(UserDirectory::Config) / "Nickvision" / "Keyring" };
            std::filesystem::create_directories(keyringDir);
            m_database = std::make_unique<SqliteDatabase>(keyringDir / (m_name + ".ring2"));
            if(m_database->isEncrypted()) // Existing Keyring
            {
                if(!m_database->unlock(credential->getPassword()))
                {
                    m_database.reset();
                }
                SqliteStatement statement{ m_database->createStatement("SELECT * FROM credentials") };
                while(statement.step() != SqliteStepResult::Done)
                {
                    m_credentials.push_back({ statement.getColumn<std::string>(0), statement.getColumn<std::string>(1), statement.getColumn<std::string>(2), statement.getColumn<std::string>(3) });
                }
            }
            else // New Keyring
            {
                if(!m_database->setPassword(credential->getPassword()))
                {
                    m_database.reset();
                }
                else if(!m_database->execute("CREATE TABLE IF NOT EXISTS credentials (name TEXT, uri TEXT, username TEXT, password TEXT)"))
                {
                    m_database.reset();
                }
            }
        }
    }

    Keyring::Keyring(Keyring&& other) noexcept
        : m_name{ std::move(other.m_name) },
        m_database{ std::move(other.m_database) },
        m_credentials{ std::move(other.m_credentials) }
    {
        std::lock_guard lock{ other.m_mutex };
        other.m_database = nullptr;
        other.m_credentials.clear();
    }

    const std::string& Keyring::getName() const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_name;
    }

    bool Keyring::isSavingToDisk() const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_database != nullptr;
    }

    const std::vector<Credential>& Keyring::getAll() const noexcept
    {
        std::lock_guard lock{ m_mutex };
        return m_credentials;
    }

    std::optional<Credential> Keyring::get(const std::string& name) noexcept
    {
        std::lock_guard lock{ m_mutex };
        for(const Credential& credential : m_credentials)
        {
            if(credential.getName() == name)
            {
                return credential;
            }
        }
        return std::nullopt;
    }

    bool Keyring::add(const Credential& credential) noexcept
    {
        std::lock_guard lock{ m_mutex };
        for(const Credential& c : m_credentials)
        {
            if(c.getName() == credential.getName())
            {
                return false;
            }
        }
        if(m_database)
        {
            SqliteStatement statement{ m_database->createStatement("INSERT INTO credentials (name, uri, username, password) VALUES (?, ?, ?, ?)") };
            statement.bind(1, credential.getName());
            statement.bind(2, credential.getUri());
            statement.bind(3, credential.getUsername());
            statement.bind(4, credential.getPassword());
            if(statement.step() != SqliteStepResult::Done)
            {
                return false;
            }
        }
        m_credentials.push_back(credential);
        return true;
    }

    bool Keyring::update(const Credential& credential) noexcept
    {
        std::lock_guard lock{ m_mutex };
        for(Credential& c : m_credentials)
        {
            if(c.getName() == credential.getName())
            {
                if(m_database)
                {
                    SqliteStatement statement{ m_database->createStatement("UPDATE credentials SET uri = ?, username = ?, password = ? WHERE name = ?") };
                    statement.bind(1, credential.getUri());
                    statement.bind(2, credential.getUsername());
                    statement.bind(3, credential.getPassword());
                    statement.bind(4, credential.getName());
                    if(statement.step() != SqliteStepResult::Done)
                    {
                        return false;
                    }
                }
                c = credential;
                return true;
            }
        }
        return false;
    }

    bool Keyring::remove(const std::string& name) noexcept
    {
        for(std::vector<Credential>::iterator it = m_credentials.begin(); it != m_credentials.end(); ++it)
        {
            if(it->getName() == name)
            {
                if(m_database)
                {
                    SqliteStatement statement{ m_database->createStatement("DELETE FROM credentials WHERE name = ?") };
                    statement.bind(1, name);
                    if(statement.step() != SqliteStepResult::Done)
                    {
                        return false;
                    }
                }
                m_credentials.erase(it);
                return true;
            }
        }
        return false;
    }

    bool Keyring::destroy() noexcept
    {
        if(m_database)
        {
            std::filesystem::path path{ m_database->getPath() };
            m_database.reset();
            m_database = nullptr;
            m_credentials.clear();
            return std::filesystem::remove(path);
        }
        return true;
    }

    Keyring& Keyring::operator=(Keyring&& other) noexcept
    {
        if(this != &other)
        {
            std::lock_guard lock{ m_mutex };
            std::lock_guard lock2{ other.m_mutex };
            m_name = std::move(other.m_name);
            m_credentials = std::move(other.m_credentials);
            m_database = std::move(other.m_database);
            other.m_database = nullptr;
            other.m_credentials.clear();
        }
        return *this;
    }
}