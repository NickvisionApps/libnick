#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <vector>
#include "passwordcontent.h"

namespace Nickvision::Aura::Keyring
{
	/**
	 * @brief A random number generator. 
	 */
	class PasswordGenerator
	{
	public:
		/**
		 * @brief Constructs a PasswordGenerator.
		 * @param contentFlags Flags of possible characters in a generated password
		 */
		PasswordGenerator(PasswordContent contentFlags = PasswordContent::Numeric | PasswordContent::Uppercase | PasswordContent::Lowercase | PasswordContent::Special);
		/**
		 * @brief Gets the flags of possible characters in a generated password.
		 * @return The flags of possible characters in a generated password
		 */
		PasswordContent getContentFlags() const;
		/**
		 * @brief Sets the flags of possible characters in a generated password.
		 * @param contentFlags Flags of possible characters in a generated password
	 	 */
		void setContentFlags(PasswordContent contentFlags);
		/**
		 * @brief Generates a new password
		 * @param length The length of the generated password
		 * @return The generated password
		 */
		std::string next(int length = 16);

	private:
		std::vector<char> m_chars;
		PasswordContent m_contentFlags;

	private:
		static std::vector<char> m_numericChars;
		static std::vector<char> m_upperChars;
		static std::vector<char> m_lowerChars;
		static std::vector<char> m_specialChars;
	};
}

#endif //PASSWORDGENERATOR_H