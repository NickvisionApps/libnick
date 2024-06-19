#include "keyring/passwordgenerator.h"
#include <cmath>
#include <ctime>
#include <random>

namespace Nickvision::Keyring
{
    PasswordGenerator::PasswordGenerator(PasswordContent contentFlags)
        : m_contentFlags{ contentFlags }
    {
        srand(time(nullptr));
    }

    PasswordContent PasswordGenerator::getContentFlags() const
    {
        return m_contentFlags;
    }

    void PasswordGenerator::setContentFlags(PasswordContent contentFlags)
    {
        m_contentFlags = contentFlags;
    }

    std::string PasswordGenerator::next(size_t length)
    {
        static std::vector<char> numericChars{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', };
        static std::vector<char> upperChars{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
        static std::vector<char> lowerChars{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
        static std::vector<char> specialChars{ '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~' };
        std::string pass;
        for(size_t i = 0; i < length; i++)
        {
            while(true)
            {
                PasswordContent randomType{ static_cast<int>(std::pow(2, rand() % 4)) };
                if((m_contentFlags & randomType) == PasswordContent::Numeric)
                {
                    pass += numericChars[rand() % numericChars.size()];
                } 
                else if((m_contentFlags & randomType) == PasswordContent::Uppercase)
                {
                    pass += upperChars[rand() % upperChars.size()];
                }  
                else if((m_contentFlags & randomType) == PasswordContent::Lowercase)
                {
                    pass += lowerChars[rand() % lowerChars.size()];
                }
                else if((m_contentFlags & randomType) == PasswordContent::Special)
                {
                    pass += specialChars[rand() % specialChars.size()];
                }
                else
                {
                    continue;
                }
                break;
            }
        }
        return pass;
    }
}