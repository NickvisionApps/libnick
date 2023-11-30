#ifndef PASSWORDCONTENT_H
#define PASSWORDCONTENT_H

namespace Nickvision::Aura::Keyring
{
	/**
	 * @brief Flags to describe the content of a password.
	 */
	enum class PasswordContent
	{
		Numeric = 1,
		Uppercase = 2,
		Lowercase = 4,
		Special = 8
	};

	inline PasswordContent operator~(PasswordContent a) 
	{ 
		return (PasswordContent)~(int)a; 
	}

	inline PasswordContent operator|(PasswordContent a, PasswordContent b) 
	{ 
		return (PasswordContent)((int)a | (int)b); 
	}

	inline PasswordContent operator&(PasswordContent a, PasswordContent b) 
	{ 
		return (PasswordContent)((int)a & (int)b);
	}

	inline PasswordContent operator^(PasswordContent a, PasswordContent b) 
	{ 
		return (PasswordContent)((int)a ^ (int)b);
	}

	inline PasswordContent& operator|=(PasswordContent& a, PasswordContent b) 
	{ 
		return (PasswordContent&)((int&)a |= (int)b);
	}

	inline PasswordContent& operator&=(PasswordContent& a, PasswordContent b) 
	{ 
		return (PasswordContent&)((int&)a &= (int)b);
	}

	inline PasswordContent& operator^=(PasswordContent& a, PasswordContent b) 
	{ 
		return (PasswordContent&)((int&)a ^= (int)b);
	}
}

#endif //PASSWORDCONTENT_H