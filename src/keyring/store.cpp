#include "keyring/store.h"
#include <stdexcept>
#include "database/sqlstatement.h"
#include "filesystem/userdirectories.h"

using namespace Nickvision::Database;
using namespace Nickvision::Filesystem;

namespace Nickvision::Keyring
{
    static std::filesystem::path getPathFromName(const std::string& name)
    {
        std::filesystem::path dir{ UserDirectories::get(UserDirectory::Config) / "Nickvision" / "Keyring" };
        std::filesystem::create_directories(dir);
        return dir / (name + ".ring");
    }

    Store::Store(const std::string& name, const std::string& password)
        : m_name{ name },
        m_password{ password },
        m_database{ std::make_shared<SqlDatabase>(getPathFromName(name), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) }
    {
        if(m_database->unlock(m_password))
        {
            if(!m_database->exec("CREATE TABLE IF NOT EXISTS credentials (id TEXT PRIMARY KEY, name TEXT, uri TEXT, username TEXT, password TEXT)"))
            {
                throw std::runtime_error("Unable to exec create table command.");
            }
        }
        else
        {
            throw std::runtime_error("Unable to key the database.");
        }
    }

    const std::string& Store::getName() const
    {
        return m_name;
    }

    const std::filesystem::path& Store::getPath() const
    {
        return m_database->getPath();
    }

    std::vector<Credential> Store::getAllCredentials() const
    {
        std::vector<Credential> creds;
        SqlStatement statement{ m_database->createStatement("SELECT * FROM credentials") };
        while(statement.step())
        {
            creds.push_back({ statement.getColumnInt(0), statement.getColumnString(1), statement.getColumnString(2), statement.getColumnString(3), statement.getColumnString(4) });
        }
        return creds;
    }

    std::optional<Credential> Store::getCredential(int id) const
    {
        SqlStatement statement{ m_database->createStatement("SELECT * FROM credentials where id = ?") };
        statement.bind(1, id);
        if(statement.step())
        {
            return Credential{ statement.getColumnInt(0), statement.getColumnString(1), statement.getColumnString(2), statement.getColumnString(3), statement.getColumnString(4) };
        }
        return std::nullopt;
    }

    std::vector<Credential> Store::getCredentials(const std::string& name) const
    {
        std::vector<Credential> creds;
        SqlStatement statement{ m_database->createStatement("SELECT * FROM credentials where name = ?") };
        statement.bind(1, name);
        while(statement.step())
        {
            creds.push_back({ statement.getColumnInt(0), statement.getColumnString(1), statement.getColumnString(2), statement.getColumnString(3), statement.getColumnString(4) });
        }
        return creds;
    }

    bool Store::addCredential(const Credential& credential)
    {
        SqlStatement statement{ m_database->createStatement("INSERT INTO credentials (id, name, uri, username, password) VALUES (?,?,?,?,?)") };
        statement.bind(1, credential.getId());
        statement.bind(2, credential.getName());
        statement.bind(3, credential.getUri());
        statement.bind(4, credential.getUsername());
        statement.bind(5, credential.getPassword());
        return !statement.step();
    }

    bool Store::updateCredential(const Credential& credential)
    {
        SqlStatement statement{ m_database->createStatement("UPDATE credentials SET name = ?, uri = ?, username = ?, password = ? where id = ?") };
        statement.bind(5, credential.getId());
        statement.bind(1, credential.getName());
        statement.bind(2, credential.getUri());
        statement.bind(3, credential.getUsername());
        statement.bind(4, credential.getPassword());
        return !statement.step();
    }

    bool Store::deleteCredential(int id)
    {
        SqlStatement statement{ m_database->createStatement("DELETE FROM credentials WHERE id = ?") };
        statement.bind(1, id);
        return !statement.step();
    }

    bool Store::destroy()
    {
        if(!m_database)
        {
            return true;
        }
        std::filesystem::path path{ m_database->getPath() };
        m_database.reset();
        m_database = nullptr;
        return std::filesystem::exists(path) ? std::filesystem::remove(path) : true;
    }

    bool Store::exists(const std::string& name)
    {
        return std::filesystem::exists(getPathFromName(name));
    }

    bool Store::destroy(const std::string& name)
    {
        std::filesystem::path path{ getPathFromName(name) };
        if (std::filesystem::exists(path))
        {
            return std::filesystem::remove(path);
        }
        return true;
    }
}