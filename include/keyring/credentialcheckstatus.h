/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Flags to describe the status of a validated credential.
 */

#ifndef CREDENTIALCHECKSTATUS_H
#define CREDENTIALCHECKSTATUS_H

namespace Nickvision::Keyring
{
    /**
     * @brief Flags to describe the status of a validated credential.
     */
    enum class CredentialCheckStatus
    {
        Valid = 1, ///< The credential is valid.
        EmptyName = 2, ///< The credential has an empty name.
        EmptyUsernamePassword = 4, ///< The credential has an empty username or password.
        InvalidUri = 8 ///< The credential has an invalid URI.
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