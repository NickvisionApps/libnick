#include "keyring/keyringdialogcontroller.h"
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Keyring
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
            m_keyring = Keyring::access(m_name, password);
            return m_keyring.has_value();
        }
        return false;
    }

    bool KeyringDialogController::disableKeyring()
    {
        if (m_keyring)
        {
            bool res{ m_keyring->destroy() };
            m_keyring = std::nullopt;
            return res;
        }
        return false;
    }

    bool KeyringDialogController::resetKeyring()
    {
        if (!m_keyring)
        {
            return Keyring::destroy(m_name);
        }
        return false;
    }

    CredentialCheckStatus KeyringDialogController::validateCredential(const Credential& credential) const
    {
        CredentialCheckStatus result = (CredentialCheckStatus)0;
        if (credential.getName().empty())
        {
            result |= CredentialCheckStatus::EmptyName;
        }
        if (credential.getUsername().empty() && credential.getPassword().empty())
        {
            result |= CredentialCheckStatus::EmptyUsernamePassword;
        }
        if (!credential.getUri().empty() && !StringHelpers::isValidUrl(credential.getUri()))
        {
            result |= CredentialCheckStatus::InvalidUri;
        }
        if ((int)result != 0)
        {
            return result;
        }
        return CredentialCheckStatus::Valid;
    }

    std::vector<Credential> KeyringDialogController::getAllCredentials() const
    {
        if (m_keyring)
        {
            return m_keyring->getAllCredentials();
        }
        return {};
    }

    bool KeyringDialogController::addCredential(const Credential& credential)
    {
        if (m_keyring && (validateCredential(credential) & CredentialCheckStatus::Valid) == CredentialCheckStatus::Valid)
        {
            return m_keyring->addCredential(credential);
        }
        return false;
    }

    bool KeyringDialogController::updateCredential(const Credential& credential)
    {
        if (m_keyring && (validateCredential(credential) & CredentialCheckStatus::Valid) == CredentialCheckStatus::Valid)
        {
            if (m_keyring->getCredential(credential.getId()))
            {
                return m_keyring->updateCredential(credential);
            }
        }
        return false;
    }

    bool KeyringDialogController::deleteCredential(int id)
    {
        if (m_keyring)
        {
            return m_keyring->deleteCredential(id);
        }
        return false;
    }
}