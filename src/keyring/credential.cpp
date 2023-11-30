#include "keyring/credential.h"
#include <functional>
#include "helpers/stringhelpers.h"

namespace Nickvision::Aura::Keyring
{
	Credential::Credential(int id, const std::string& name, const std::string& uri, const std::string& username, const std::string& password)
		: m_id{ id },
		m_name{ name },
		m_uri{ uri },
		m_username{ username },
		m_password{ password }
	{

	}

	Credential::Credential(const std::string& name, const std::string& uri, const std::string& username, const std::string& password)
		: m_id{ (int)std::hash<std::string>()(StringHelpers::newGuid()) },
		m_name{ name },
		m_uri{ uri },
		m_username{ username },
		m_password{ password }
	{

	}

	int Credential::getId() const
	{
		return m_id;
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
		return m_id < compare.m_id;
	}

	bool Credential::operator>(const Credential& compare) const
	{
		return m_id > compare.m_id;
	}

	bool Credential::operator==(const Credential& compare) const
	{
		return m_id == compare.m_id;
	}

	bool Credential::operator!=(const Credential& compare) const
	{
		return m_id != compare.m_id;
	}

	PasswordStrength Credential::getPasswordStrength(const std::string& password)
	{
		if (password.empty())
		{
			return PasswordStrength::Blank;
		}
		if (password.size() < 4)
		{
			return PasswordStrength::VeryWeak;
		}
		int strength = 1;
		bool containsDigit = false;
		bool containsLetter = false;
		bool containsSymbol = false;
		if (password.size() >= 8)
		{
			strength++;
		}
		for (char ch : password)
		{
			if (!containsDigit && std::isdigit(ch))
			{
				strength++;
				containsDigit = true;
			}
			if (!containsLetter && std::isalpha(ch))
			{
				strength++;
				containsLetter = true;
			}
			if (!containsSymbol && !std::isalnum(ch))
			{
				strength++;
				containsSymbol = true;
			}
		}
		return (PasswordStrength)strength;
	}
}