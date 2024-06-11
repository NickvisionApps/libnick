#ifndef CODEHELPERS_H
#define CODEHELPERS_H

#include <string>

namespace Nickvision::Helpers::CodeHelpers
{
    /**
     * @brief Get the last system api call error message.
     * @return The last system error message
     */
    std::string getLastSystemError();
    /**
     * @brief Unmoves a value.
     * @param t An rvalue of T
     * @return The lvalue of t
     */
    template<typename T>
    const T& unmove(T&& t)
    {
        return t;
    }
}

#endif //CODEHELPERS_H
