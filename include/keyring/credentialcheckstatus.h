#ifndef CREDENTIALCHECKSTATUS_H
#define CREDENTIALCHECKSTATUS_H

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

    constexpr CredentialCheckStatus operator~(CredentialCheckStatus a)
    {
        return static_cast<CredentialCheckStatus>(~static_cast<int>(a));
    }

    constexpr CredentialCheckStatus operator|(CredentialCheckStatus a, CredentialCheckStatus b)
    {
        return static_cast<CredentialCheckStatus>(static_cast<int>(a) | static_cast<int>(b));
    }

    constexpr CredentialCheckStatus operator&(CredentialCheckStatus a, CredentialCheckStatus b)
    {
        return static_cast<CredentialCheckStatus>(static_cast<int>(a) & static_cast<int>(b));
    }

    constexpr CredentialCheckStatus operator^(CredentialCheckStatus a, CredentialCheckStatus b)
    {
        return static_cast<CredentialCheckStatus>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    inline CredentialCheckStatus& operator|=(CredentialCheckStatus& a, CredentialCheckStatus b)
    {
        return reinterpret_cast<CredentialCheckStatus&>(reinterpret_cast<int&>(a) |= static_cast<int>(b));
    }

    inline CredentialCheckStatus& operator&=(CredentialCheckStatus& a, CredentialCheckStatus b)
    {
        return reinterpret_cast<CredentialCheckStatus&>(reinterpret_cast<int&>(a) &= static_cast<int>(b));
    }

    inline CredentialCheckStatus& operator^=(CredentialCheckStatus& a, CredentialCheckStatus b)
    {
        return reinterpret_cast<CredentialCheckStatus&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b));
    }
}

#endif //CREDENTIALCHECKSTATUS_H