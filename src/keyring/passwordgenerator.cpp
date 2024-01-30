#include "keyring/passwordgenerator.h"
#include <ctime>
#include <random>

namespace Nickvision::Keyring
{
    std::vector<char> PasswordGenerator::m_numericChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    std::vector<char> PasswordGenerator::m_upperChars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
    std::vector<char> PasswordGenerator::m_lowerChars = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    std::vector<char> PasswordGenerator::m_specialChars = { '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~' };

    PasswordGenerator::PasswordGenerator(PasswordContent contentFlags)
    {
        srand((unsigned)time(0));
        setContentFlags(contentFlags);
    }

    PasswordContent PasswordGenerator::getContentFlags() const
    {
        return m_contentFlags;
    }

    void PasswordGenerator::setContentFlags(PasswordContent contentFlags)
    {
        m_contentFlags = contentFlags;
        m_chars.clear();
        if ((m_contentFlags & PasswordContent::Numeric) == PasswordContent::Numeric)
        {
            m_chars.insert(m_chars.end(), m_numericChars.begin(), m_numericChars.end());
        }
        if ((m_contentFlags & PasswordContent::Uppercase) == PasswordContent::Uppercase)
        {
            m_chars.insert(m_chars.end(), m_upperChars.begin(), m_upperChars.end());
        }
        if ((m_contentFlags & PasswordContent::Lowercase) == PasswordContent::Lowercase)
        {
            m_chars.insert(m_chars.end(), m_lowerChars.begin(), m_lowerChars.end());
        }
        if ((m_contentFlags & PasswordContent::Special) == PasswordContent::Special)
        {
            m_chars.insert(m_chars.end(), m_specialChars.begin(), m_specialChars.end());
        }
    }

    std::string PasswordGenerator::next(size_t length)
    {
        std::string pass;
        for(size_t i = 0; i < length; i++)
        {
            pass += m_chars[rand() % m_chars.size()];
        }
        return pass;
    }
}