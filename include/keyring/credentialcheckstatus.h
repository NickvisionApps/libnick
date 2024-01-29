#ifndef CREDENTIALCHECKSTATUS_H
#define CREDENTIALCHECKSTATUS_H

#include "enumflags.h"

namespace Nickvision::Keyring
{
    /**
     * @brief Flags to describe the status of a validated credential
     */
    enum class CredentialCheckStatus
    {
        Valid = 1,
        EmptyName = 2,
        EmptyUsernamePassword = 4,
        InvalidUri = 8
    };

    DEFINE_ENUM_FLAG_OPERATORS(CredentialCheckStatus);
}

#endif //CREDENTIALCHECKSTATUS_H