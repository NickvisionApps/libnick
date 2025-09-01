#ifndef SIZEHELPERS_H
#define SIZEHELPERS_H

#include <type_traits>

#define FACTOR 1024

namespace Nickvision::Helpers::SizeHelpers
{
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    template<Numeric T>
    constexpr T bytesToKilobytes(const T value) noexcept
    {
        return value / FACTOR;
    }

    template<Numeric T>
    constexpr T kilobytesToBytes(const T value) noexcept
    {
        return value * FACTOR;
    }

    template<Numeric T>
    constexpr T bytesToMegabytes(const T value) noexcept
    {
        return value / (FACTOR * FACTOR);
    }

    template<Numeric T>
    constexpr T megabytesToBytes(const T value) noexcept
    {
        return value * (FACTOR * FACTOR);
    }

    template<Numeric T>
    constexpr T bytesToGigabytes(const T value) noexcept
    {
        return value / (FACTOR * FACTOR * FACTOR);
    }

    template<Numeric T>
    constexpr T gigabytesToBytes(const T value) noexcept
    {
        return value * (FACTOR * FACTOR * FACTOR);
    }
}

#endif //SIZEHELPERS_H