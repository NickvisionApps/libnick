#ifndef PASSWORDCONTENT_H
#define PASSWORDCONTENT_H

#include "enumflags.h"

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

    DEFINE_ENUM_FLAG_OPERATORS(PasswordContent);
}

#endif //PASSWORDCONTENT_H