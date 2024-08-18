#include "keyring/credential.h"
#include "keyring/passwordstrength.h"
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Keyring
{
    Credential::Credential(const std::string& name, const std::string& uri, const std::string& username, const std::string& password)
        : m_name{ name },
        m_uri{ uri },
        m_username{ username },
        m_password{ password }
    {

    }

    const std::string& Credential::getName() const
    {
        return m_name;
    }

    void Credential::setName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& Credential::getUri() const
    {
        return m_uri;
    }

    void Credential::setUri(const std::string& uri)
    {
        m_uri = uri;
    }

    const std::string& Credential::getUsername() const
    {
        return m_username;
    }

    void Credential::setUsername(const std::string& username)
    {
        m_username = username;
    }

    const std::string& Credential::getPassword() const
    {
        return m_password;
    }

    void Credential::setPassword(const std::string& password)
    {
        m_password = password;
    }

    bool Credential::operator<(const Credential& compare) const
    {
        return m_name < compare.m_name;
    }

    bool Credential::operator>(const Credential& compare) const
    {
        return m_name > compare.m_name;
    }

    bool Credential::operator==(const Credential& compare) const
    {
        return m_name == compare.m_name;
    }

    bool Credential::operator!=(const Credential& compare) const
    {
        return m_name != compare.m_name;
    }

    std::ostream& operator<<(std::ostream& os, const Credential& credential)
    {
        os << "[CRED: " << credential.getName() << "] " << std::endl;
        os << "Uri: " << credential.getUri() << std::endl;
        os << "Username: " << credential.getUsername() << std::endl;
        os << "Password: " << credential.getPassword() << std::endl;
        os << "Strength: " << (int)getPasswordStrength(credential.getPassword()) << std::endl;
        return os;
    }
}