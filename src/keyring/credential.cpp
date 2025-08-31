#include "keyring/credential.h"

namespace Nickvision::Keyring
{
    Credential::Credential(const std::string& name, const std::string& uri, const std::string& username, const std::string& password) noexcept
        : m_name{ name },
        m_uri{ uri },
        m_username{ username },
        m_password{ password }
    {

    }

    const std::string& Credential::getName() const noexcept
    {
        return m_name;
    }

    void Credential::setName(const std::string& name) noexcept
    {
        m_name = name;
    }

    const std::string& Credential::getUri() const noexcept
    {
        return m_uri;
    }

    void Credential::setUri(const std::string& uri) noexcept
    {
        m_uri = uri;
    }

    const std::string& Credential::getUsername() const noexcept
    {
        return m_username;
    }

    void Credential::setUsername(const std::string& username) noexcept
    {
        m_username = username;
    }

    const std::string& Credential::getPassword() const noexcept
    {
        return m_password;
    }

    void Credential::setPassword(const std::string& password) noexcept
    {
        m_password = password;
    }
}