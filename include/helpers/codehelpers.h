#ifndef CODEHELPERS_H
#define CODEHELPERS_H

namespace Nickvision::CodeHelpers
{
    template<typename T>
    const T& unmove(T&& t)
    {
        return t;
    }
}

#endif //CODEHELPERS_H
