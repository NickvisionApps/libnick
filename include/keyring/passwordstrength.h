#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

namespace Nickvision::Aura::Keyring
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
}

#endif //PASSWORDSTRENGTH_H