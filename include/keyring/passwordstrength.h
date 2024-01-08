#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

#include <string>

namespace Nickvision::Keyring
{
    /**
     * @brief Strengths for a password. 
     */
    enum class PasswordStrength
    {
        Blank = 0,
        VeryWeak,
        Weak,
        Medium,
        Strong,
        VeryStrong
    };

    /**
     * @brief Calculates the strength of a given password.
     * @param password The password to measure the strength of
     * @return The strength of the password
     */
    PasswordStrength getPasswordStrength(const std::string& password) noexcept;
}

#endif //PASSWORDSTRENGTH_H