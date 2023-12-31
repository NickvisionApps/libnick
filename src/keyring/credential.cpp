#include "keyring/credential.h"
#include <functional>
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura::Keyring
{
	Credential::Credential(int id, const std::string& name, const std::string& uri, const std::string& username, const std::string& password) noexcept
		: m_id{ id },
		m_name{ name },
		m_uri{ uri },
		m_username{ username },
		m_password{ password }
	{

	}

	Credential::Credential(const std::string& name, const std::string& uri, const std::string& username, const std::string& password) noexcept
		: m_id{ static_cast<int>(std::hash<std::string>()(StringHelpers::newGuid())) },
		m_name{ name },
		m_uri{ uri },
		m_username{ username },
		m_password{ password }
	{

	}

	int Credential::getId() const noexcept
	{
		return m_id;
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

	bool Credential::operator<(const Credential& compare) const noexcept
	{
		return m_id < compare.m_id;
	}

	bool Credential::operator>(const Credential& compare) const noexcept
	{
		return m_id > compare.m_id;
	}

	bool Credential::operator==(const Credential& compare) const noexcept
	{
		return m_id == compare.m_id;
	}

	bool Credential::operator!=(const Credential& compare) const noexcept
	{
		return m_id != compare.m_id;
	}

	std::ostream& operator<<(std::ostream& os, const Credential& credential) noexcept
	{
		os << "[CRED: " << credential.getName() << "] " << std::endl;
		os << "Uri: " << credential.getUri() << std::endl;
		os << "Username: " << credential.getUsername() << std::endl;
		os << "Password: " << credential.getPassword() << std::endl;
		os << "Strength: " << (int)getPasswordStrength(credential.getPassword()) << std::endl;
		return os;
	}
}