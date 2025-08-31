#include "keyring/passwordgenerator.h"
#include <cmath>
#include <cstdlib>

namespace Nickvision::Keyring
{
    PasswordGenerator::PasswordGenerator(PasswordContent contentFlags) noexcept
        : m_contentFlags{ contentFlags },
        m_randomEngine{ m_randomDevice() }
    {

    }

    PasswordContent PasswordGenerator::getContentFlags() const noexcept
    {
        return m_contentFlags;
    }

    void PasswordGenerator::setContentFlags(PasswordContent contentFlags) noexcept
    {
        m_contentFlags = contentFlags;
    }

    std::string PasswordGenerator::next(size_t length) noexcept
    {
        static std::vector<char> numericChars{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', };
        static std::vector<char> upperChars{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
        static std::vector<char> lowerChars{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
        static std::vector<char> specialChars{ '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~' };
        static std::uniform_int_distribution<int> typeDistribution{ 0, 4 };
        static std::uniform_int_distribution<size_t> numericDistribution{ 0, numericChars.size() - 1 };
        static std::uniform_int_distribution<size_t> upperDistribution{ 0, upperChars.size() - 1 };
        static std::uniform_int_distribution<size_t> lowerDistribution{ 0, lowerChars.size() - 1 };
        static std::uniform_int_distribution<size_t> specialDistribution{ 0, specialChars.size() - 1 };
        std::string password;
        for(size_t i = 0; i < length; i++)
        {
            while(true)
            {
                PasswordContent randomType{ static_cast<int>(std::pow(2, typeDistribution(m_randomEngine))) };
                if((m_contentFlags & randomType) == PasswordContent::Numeric)
                {
                    password += numericChars[numericDistribution(m_randomEngine)];
                } 
                else if((m_contentFlags & randomType) == PasswordContent::Uppercase)
                {
                    password += upperChars[upperDistribution(m_randomEngine)];
                }  
                else if((m_contentFlags & randomType) == PasswordContent::Lowercase)
                {
                    password += lowerChars[lowerDistribution(m_randomEngine)];
                }
                else if((m_contentFlags & randomType) == PasswordContent::Special)
                {
                    password += specialChars[specialDistribution(m_randomEngine)];
                }
                else if((m_contentFlags & randomType) == PasswordContent::Space)
                {
                    password += ' ';
                }
                else
                {
                    continue;
                }
                break;
            }
        }
        return password;
    }
}