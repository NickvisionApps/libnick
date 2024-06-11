#ifndef PASSWORDCONTENT_H
#define PASSWORDCONTENT_H

namespace Nickvision::Keyring
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
        return static_cast<PasswordContent>(~static_cast<int>(a));
    }

    inline PasswordContent operator|(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline PasswordContent operator&(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) & static_cast<int>(b));
    }

    inline PasswordContent operator^(PasswordContent a, PasswordContent b)
    {
        return static_cast<PasswordContent>(static_cast<int>(a) ^ static_cast<int>(b));
    }

    inline PasswordContent& operator|=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) |= static_cast<int>(b));
    }

    inline PasswordContent& operator&=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) &= static_cast<int>(b));
    }

    inline PasswordContent& operator^=(PasswordContent& a, PasswordContent b)
    {
        return reinterpret_cast<PasswordContent&>(reinterpret_cast<int&>(a) ^= static_cast<int>(b));
    }
}

#endif //PASSWORDCONTENT_H