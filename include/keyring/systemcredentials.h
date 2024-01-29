#ifndef SYSTEMCREDENTIALS_H
#define SYSTEMCREDENTIALS_H

#include <optional>
#include <string>
#include "credential.h"

namespace Nickvision::Keyring::SystemCredentials
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
     * @return The new Credential object, if successful
     */
    std::optional<Credential> addCredential(const std::string& name);
    /**
     * @brief Adds a new credential to the system's credential manager. On Linux, only the name and password of a credential will be stored. On Windows, all fields of a credential will be stored.
     * @param name The new credential object
     * @return True if successful, else false
     */
    bool addCredential(const Credential& credential);
    /**
     * @brief Updates a credential in the system's credential manager. On Linux, only the name and password of a credential will be stored. On Windows, all fields of a credential will be stored.
     * @param name The updated credential object
     * @return True if successful, else false
     */
    bool updateCredential(const Credential& credential);
    /**
     * @brief Deletes a credential from the system's credential manager.
     * @param name The name of the credential to delete
     * @return True if successful, else false
     */
    bool deleteCredential(const std::string& name);
}

#endif //SYSTEMCREDENTIALS_H