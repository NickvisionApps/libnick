#include "keyring/keyring.h"
#include "keyring/systemcredentials.h"

namespace Nickvision::Aura::Keyring
{
	Keyring::Keyring(const Store& store)
		: m_store{ store }
	{

	}

	const std::string& Keyring::getName() const
	{
		return m_store.getName();
	}

	std::vector<Credential> Keyring::getAllCredentials() const
	{
		return m_store.getAllCredentials();
	}

	std::optional<Credential> Keyring::getCredential(int id) const
	{
		return m_store.getCredential(id);
	}

	std::vector<Credential> Keyring::getCredentials(const std::string& name) const
	{
		return m_store.getCredentials(name);
	}

	bool Keyring::addCredential(const Credential& credential)
	{
		return m_store.addCredential(credential);
	}

	bool Keyring::updateCredential(const Credential& credential)
	{
		return m_store.updateCredential(credential);
	}

	bool Keyring::deleteCredential(int id)
	{
		return m_store.deleteCredential(id);
	}

	bool Keyring::destroy()
	{
		if (m_store.destroy())
		{
			return SystemCredentials::deleteCredential(m_store.getName());
		}
		return false;
	}

	std::optional<Keyring> Keyring::access(const std::string& name, std::string password)
	{
		//If password empty, get password from system credential store
		if (password.empty())
		{
			std::optional<Credential> cred{ SystemCredentials::getCredential(name) };
			password = cred ? cred->getPassword() : "";
		}
		//If password not empty (a.k.a user-provided or system-provided), get store
		if (!password.empty())
		{
			//Load store
			std::optional<Store> storeLoad{ Store::load(name, password) };
			if (storeLoad)
			{
				return { { *storeLoad } };
			}
			//Store unable to be loaded, create a new one
			else
			{
				std::optional<Store> storeCreate{ Store::create(name, password) };
				if (storeCreate)
				{
					return { { *storeCreate } };
				}
				else
				{
					return std::nullopt;
				}
			}
		}
		//Create a new store with a new password from the system credential store
		else
		{
			std::optional<Credential> cred{ SystemCredentials::addCredential(name) };
			if (cred)
			{
				std::optional<Store> storeCreate{ Store::create(name, cred->getPassword()) };
				if (storeCreate)
				{
					return { { *storeCreate } };
				}
				else
				{
					return std::nullopt;
				}
			}
		}
		return std::nullopt;
	}

	bool Keyring::exists(const std::string& name)
	{
		return Store::exists(name);
	}

	bool Keyring::destroy(const std::string& name)
	{
		if (Store::destroy(name))
		{
			return SystemCredentials::deleteCredential(name);
		}
		return false;
	}
}