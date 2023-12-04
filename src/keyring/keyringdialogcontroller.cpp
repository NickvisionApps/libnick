#include "keyring/keyringdialogcontroller.h"

namespace Nickvision::Aura::Keyring
{
	KeyringDialogController::KeyringDialogController(const std::string& name, const std::optional<Keyring>& keyring)
		: m_name{ name },
		m_keyring{ keyring }
	{

	}

	const std::optional<Keyring>& KeyringDialogController::getKeyring()
	{
		return m_keyring;
	}

	bool KeyringDialogController::isEnabled() const
	{
		return m_keyring.has_value();
	}

	bool KeyringDialogController::isValid() const
	{
		return !(!m_keyring && Keyring::exists(m_name));
	}

	bool KeyringDialogController::enableKeyring(const std::string& password)
	{
		if (!m_keyring)
		{
			/*
			m_keyring = Keyring::access(m_name, password);
			return m_keyring.has_value();
			*/
		}
		return false;
	}
}