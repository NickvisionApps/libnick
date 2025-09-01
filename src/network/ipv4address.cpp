#include "network/ipv4address.h"
#include <format>
#include <limits>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Network
{
    IPv4Address::IPv4Address(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth) noexcept
        : m_first{ first },
        m_second{ second },
        m_third{ third },
        m_fourth{ fourth }
    {

    }

    IPv4Address::IPv4Address(unsigned long address) noexcept
        : m_first{ static_cast<unsigned char>((address >> 24) & 0xFF) },
        m_second{ static_cast<unsigned char>((address >> 16) & 0xFF) },
        m_third{ static_cast<unsigned char>((address >> 8) & 0xFF) },
        m_fourth{ static_cast<unsigned char>(address & 0xFF) }
    {
        
    }

    std::optional<IPv4Address> IPv4Address::parse(const std::string& address) noexcept
    {
        if(address == "localhost")
        {
            return IPv4Address{ 127, 0, 0, 1 };
        }
        std::vector<std::string> parts{ StringHelpers::split(address, ".") };
        if(parts.size() != 4)
        {
            return std::nullopt;
        }
        try
        {
            int first{ std::stoi(parts[0]) };
            int second{ std::stoi(parts[1]) };
            int third{ std::stoi(parts[2]) };
            int fourth{ std::stoi(parts[3]) };
            unsigned char max{ std::numeric_limits<unsigned char>::max() };
            if(first < 0 || first > max || second < 0 || second > max || third < 0 || third > max || fourth < 0 || fourth > max)
            {
                return std::nullopt;
            }
            return IPv4Address{ static_cast<unsigned char>(first), static_cast<unsigned char>(second), static_cast<unsigned char>(third), static_cast<unsigned char>(fourth) };
        }
        catch(...)
        {
            return std::nullopt;
        }
    }

    unsigned char IPv4Address::getFirst() const noexcept
    {
        return m_first;
    }

    unsigned char IPv4Address::getSecond() const noexcept
    {
        return m_second;
    }

    unsigned char IPv4Address::getThird() const noexcept
    {
        return m_third;
    }

    unsigned char IPv4Address::getFourth() const noexcept
    {
        return m_fourth;
    }

    unsigned long IPv4Address::getNetworkByteOrder() const noexcept
    {
        return (static_cast<unsigned long>(m_first) << 24) | (static_cast<unsigned long>(m_second) << 16) | (static_cast<unsigned long>(m_third) << 8) | static_cast<unsigned long>(m_fourth);
    }

    std::string IPv4Address::str() const noexcept
    {
        return std::format("{}.{}.{}.{}", static_cast<int>(m_first), static_cast<int>(m_second), static_cast<int>(m_third), static_cast<int>(m_fourth));
    }
}
