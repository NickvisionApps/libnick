#include "network/ipv4address.h"
#include <limits>
#include <sstream>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Network
{
    IPv4Address::IPv4Address(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth)
        : m_first(first),
        m_second(second),
        m_third(third),
        m_fourth(fourth)
    {

    }

    unsigned char IPv4Address::getFirst() const
    {
        return m_first;
    }

    unsigned char IPv4Address::getSecond() const
    {
        return m_second;
    }

    unsigned char IPv4Address::getThird() const
    {
        return m_third;
    }

    unsigned char IPv4Address::getFourth() const
    {
        return m_fourth;
    }

    std::string IPv4Address::str() const
    {
        return std::to_string(static_cast<int>(m_first)) + "." + std::to_string(static_cast<int>(m_second)) + "." + std::to_string(static_cast<int>(m_third)) + "." + std::to_string(static_cast<int>(m_fourth));
    }

    std::optional<IPv4Address> IPv4Address::parse(const std::string& address)
    {
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
}