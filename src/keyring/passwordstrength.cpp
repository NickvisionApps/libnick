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
		int strength = 0;
		bool containsDigit = false;
		bool containsLower = false;
		bool containsUpper = false;
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
			if (!containsLower && std::isalpha(ch) && std::islower(ch))
			{
				strength++;
				containsLower = true;
			}
			if (!containsUpper && std::isalpha(ch) && std::isupper(ch))
			{
				strength++;
				containsUpper = true;
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