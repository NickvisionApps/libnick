#include "keyring/keyring.h"
#include "database/sqlstatement.h"
#include "filesystem/userdirectories.h"
#include "keyring/systemcredentials.h"

using namespace Nickvision::Database;
using namespace Nickvision::Filesystem;

namespace Nickvision::Keyring
{
    Keyring::Keyring(const std::string& name)
        : m_name{ name },
        m_database{ nullptr }
    {
        std::optional<Credential> credential{ SystemCredentials::getCredential(name) };
        if(!credential)
        {
            credential = SystemCredentials::addCredential(name);
        }
        if(credential)
        {
            std::filesystem::path keyringDir{ UserDirectories::get(UserDirectory::Config) / "Nickvision" / "Keyring" };
            std::filesystem::create_directories(keyringDir);
            m_database = std::make_shared<SqlDatabase>(keyringDir / (m_name + ".ring2"), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
            if(m_database->unlock(credential->getPassword()))
            {
                if(!m_database->exec("CREATE TABLE IF NOT EXISTS credentials (name TEXT, uri TEXT, username TEXT, password TEXT)"))
                {
                    m_database.reset();
                }
            }
        }
    }

    const std::string& Keyring::getName() const
    {
        return m_name;
    }

    bool Keyring::isSavingToDisk() const
    {
        return m_database != nullptr;
    }

    const std::vector<Credential>& Keyring::getCredentials() const
    {
        return m_credentials;
    }

    std::optional<Credential> Keyring::getCredential(const std::string& name)
    {
        for(const Credential& credential : m_credentials)
        {
            if(credential.getName() == name)
            {
                return credential;
            }
        }
        return std::nullopt;
    }

    bool Keyring::addCredential(const Credential& credential)
    {
        for(const Credential& c : m_credentials)
        {
            if(c.getName() == credential.getName())
            {
                return false;
            }
        }
        m_credentials.push_back(credential);
        if(m_database)
        {
            SqlStatement statement{ m_database->createStatement("INSERT INTO credentials (name, uri, username, password) VALUES (?, ?, ?, ?)") };
            statement.bind(1, credential.getName());
            statement.bind(2, credential.getUri());
            statement.bind(3, credential.getUsername());
            statement.bind(4, credential.getPassword());
            return !statement.step();
        }
        return true;
    }

    bool Keyring::updateCredential(const Credential& credential)
    {
        for(Credential& c : m_credentials)
        {
            if(c.getName() == credential.getName())
            {
                c = credential;
                if(m_database)
                {
                    SqlStatement statement{ m_database->createStatement("UPDATE credentials SET uri = ?, username = ?, password = ? WHERE name = ?") };
                    statement.bind(1, credential.getUri());
                    statement.bind(2, credential.getUsername());
                    statement.bind(3, credential.getPassword());
                    statement.bind(4, credential.getName());
                    return !statement.step();
                }
                return true;
            }
        }
        return false;
    }

    bool Keyring::deleteCredential(const std::string& name)
    {
        for(auto it = m_credentials.begin(); it != m_credentials.end(); ++it)
        {
            if(it->getName() == name)
            {
                m_credentials.erase(it);
                if(m_database)
                {
                    SqlStatement statement{ m_database->createStatement("DELETE FROM credentials WHERE name = ?") };
                    statement.bind(1, name);
                    return !statement.step();
                }
                return true;
            }
        }
        return false;
    }

    bool Keyring::destroy()
    {
        if(m_database)
        {
            std::filesystem::path path{ m_database->getPath() };
            m_database.reset();
            m_database = nullptr;
            std::filesystem::remove(path);
        }
        return true;
    }
}