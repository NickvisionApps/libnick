#include "keyring/passwordstrength.h"

namespace Nickvision::Aura::Keyring
{
	PasswordStrength getPasswordStrength(const std::string& password)
	{
		if (password.empty())
		{
			return PasswordStrength::Blank;
		}
		if (password.size() <= 6)
		{
			return PasswordStrength::VeryWeak;
		}
		int strength = 1;
		bool containsDigit = false;
		bool containsLetter = false;
		bool containsSymbol = false;
		if (password.size() >= 12)
		{
			strength++;
		}
		for (char ch : password)
		{
			if (!containsDigit && std::isdigit(ch))
			{
				strength++;
				containsDigit = true;
			}
			if (!containsLetter && std::isalpha(ch))
			{
				strength++;
				containsLetter = true;
			}
			if (!containsSymbol && !std::isalnum(ch))
			{
				strength++;
				containsSymbol = true;
			}
		}
		return (PasswordStrength)strength;
	}
}