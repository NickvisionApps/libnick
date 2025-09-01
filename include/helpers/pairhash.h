#ifndef PAIRHASH_H
#define PAIRHASH_H

#include <utility>
#include "codehelpers.h"

namespace Nickvision::Helpers
{
    /**
     * @brief A hash value for std::pair.
     */
    class PairHash
    {
    public:
        /**
         * @brief Constructs a PairHash.
         */
        PairHash() noexcept = default;
        /**
         * @brief The call operator.
         * @param p The std::pair to determine a hash value for
         * @return The hash value of the std::pair
         */
        template <class T1, class T2>
        size_t operator()(const std::pair<T1,T2>& p) const noexcept
        {
            return CodeHelpers::combineHash(std::hash<T1>()(p.first), std::hash<T2>()(p.second));
        }
    };
}

#endif //PAIRHASH_H
