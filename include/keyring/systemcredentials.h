#ifndef SYSTEMCREDENTIALS_H
#define SYSTEMCREDENTIALS_H

#include <optional>
#include <string>
#include "credential.h"

namespace Nickvision::Aura::Keyring::SystemCredentials
{
	/**
	 * @brief Gets a credential from the system's credential manager.
	 * @param name The name of the credential
	 * @return The Credential object, if found
	 */
	std::optional<Credential> getCredential(const std::string& name);
	/**
	 * @brief Adds a new credential with a random password to the system's credential manager.
	 * @param name The name of the credential
	 * @return The new Credential object if successful
	 */
	std::optional<Credential> addNewCredential(const std::string& name);
	/**
	 * @brief Deletes a Credential from the system's credential manager 
	 */
	void deleteCredential(const Credential& name);
}

#endif //SYSTEMCREDENTIALS_H