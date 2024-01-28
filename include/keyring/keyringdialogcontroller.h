#ifndef KEYRINGDIALOGCONTROLLER_H
#define KEYRINGDIALOGCONTROLLER_H

#include <optional>
#include <string>
#include <vector>
#include "credential.h"
#include "credentialcheckstatus.h"
#include "keyring.h"

namespace Nickvision::Keyring
{
	/**
	 * @brief A controller for a KeyringDialog. 
	 */
	class KeyringDialogController
	{
	public:
		/**
		 * @brief Constructs a KeyringDialogController.
		 * @param name The name of the controller
		 * @param keyring The keyring managed by the controller, if available
		 */
		KeyringDialogController(const std::string& name, const std::optional<Keyring>& keyring);
		/**
		 * @brief Gets the keyring managed by the controller, if available.
		 * @return The keyring if available, else std::nullopt
		 */
		const std::optional<Keyring>& getKeyring();
		/**
		 * @brief Gets whether or not the keyring is enabled (unlocked).
		 * @return True if enabled, else false
		 */
		bool isEnabled() const;
		/**
		 * @brief Gets whether or not the keyring state is valid.
		 * @return True if valid, else false
		 */
		bool isValid() const;
		/**
		 * @brief Enables the keyring.
		 * @param password The password of the keyring
		 * @return True if successful, else false
		 */
		bool enableKeyring(const std::string& password = "");
		/**
		 * @brief Disables the keyring and destroys its data.
		 * @return True if successful, else false
		 */
		bool disableKeyring();
		/**
		 * @brief Resets the keyring and destroys its data. To be used only if the keyring is locked. If unlocked, use disableKeyring() .
		 * @return True if successful, else false
		 */
		bool resetKeyring();
		/**
		 * @brief Validates a Credential object.
		 * @param credential The credential to validate
		 * @return CredentialCheckStatus
		 */
		CredentialCheckStatus validateCredential(const Credential& credential) const;
		/**
		 * @brief Gets all credentials in the keyring.
		 * @return The list of all credentials
		 */
		std::vector<Credential> getAllCredentials() const;
		/**
		 * @brief Adds a credential to the keyring.
		 * @param credential The Credential to add
		 * @return True if successful, else false
		 */
		bool addCredential(const Credential& credential);
		/**
		 * @brief Updates a credential in the keyring.
		 * @param credential The Credential to update
		 * @return True if successful, else false
		 */
		bool updateCredential(const Credential& credential);
		/**
		 * @brief Deletes a credential from the keyring.
		 * @param id The id of the credential to delete
		 * @return True if successful, else false
		 */
		bool deleteCredential(int id);

	private:
		std::string m_name;
		std::optional<Keyring> m_keyring;
	};
}

#endif //KEYRINGDIALOGCONTROLLER_H