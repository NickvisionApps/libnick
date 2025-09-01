#include "network/macaddress.h"
#include <format>
#include <limits>
#include "helpers/stringhelpers.h"

using namespace Nickvision::Helpers;

namespace Nickvision::Network
{
    MacAddress::MacAddress(unsigned char oui1, unsigned char oui2, unsigned char oui3, unsigned char nic1, unsigned char nic2, unsigned char nic3) noexcept
        : m_oui1{ oui1 },
        m_oui2{ oui2 },
        m_oui3{ oui3 },
        m_nic1{ nic1 },
        m_nic2{ nic2 },
        m_nic3{ nic3 }
    {

    }

    std::optional<MacAddress> MacAddress::parse(const std::string& address) noexcept
    {
        std::vector<std::string> parts{ StringHelpers::split(address, ":") };
        if(parts.size() != 6)
        {
            return std::nullopt;
        }
        try
        {
            int oui1{ std::stoi(parts[0], nullptr, 16) };
            int oui2{ std::stoi(parts[1], nullptr, 16) };
            int oui3{ std::stoi(parts[2], nullptr, 16) };
            int nic1{ std::stoi(parts[3], nullptr, 16) };
            int nic2{ std::stoi(parts[4], nullptr, 16) };
            int nic3{ std::stoi(parts[5], nullptr, 16) };
            unsigned char max{ std::numeric_limits<unsigned char>::max() };
            if(oui1 < 0 || oui1 > max || oui2 < 0 || oui2 > max || oui3 < 0 || oui3 > max || nic1 < 0 || nic1 > max || nic2 < 0 || nic2 > max || nic3 < 0 || nic3 > max)
            {
                return std::nullopt;
            }
            return MacAddress{ static_cast<unsigned char>(oui1), static_cast<unsigned char>(oui2), static_cast<unsigned char>(oui3), static_cast<unsigned char>(nic1), static_cast<unsigned char>(nic2), static_cast<unsigned char>(nic3) };
        }
        catch(...)
        {
            return std::nullopt;
        }
    }

    unsigned char MacAddress::getFirst() const noexcept
    {
        return m_oui1;
    }

    unsigned char MacAddress::getSecond() const noexcept
    {
        return m_oui2;
    }

    unsigned char MacAddress::getThird() const noexcept
    {
        return m_oui3;
    }

    unsigned char MacAddress::getFourth() const noexcept
    {
        return m_nic1;
    }

    unsigned char MacAddress::getFifth() const noexcept
    {
        return m_nic2;
    }

    unsigned char MacAddress::getSixth() const noexcept
    {
        return m_nic3;
    }

    std::string MacAddress::str() const noexcept
    {
        return StringHelpers::upper(std::format("{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}", m_oui1, m_oui2, m_oui3, m_nic1, m_nic2, m_nic3));
    }
}