/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A model of an IPv4 address.
 */

#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H

#include <optional>
#include <string>

namespace Nickvision::Network
{
    /**
     * @brief A model of an IPv4 address.
     */
    class IPv4Address
    {
    public:
        /**
         * @brief Constructs an IPv4Address.
         * @param first The first byte of the address
         * @param second The second byte of the address
         * @param third The third byte of the address
         * @param fourth The fourth byte of the address
         */
        IPv4Address(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth) noexcept;
        /**
         * @brief Constructs an IPv4Address.
         * @param address The address as a long
         */
        IPv4Address(unsigned long address) noexcept;
        /**
         * @brief Parses an IPv4Address from a string.
         * @param address The string address to parse
         * @return The parsed address if successful, else std::nullopt
         */
        static std::optional<IPv4Address> parse(const std::string& address) noexcept;
        /**
         * @brief Gets the first byte of the address.
         * @return The first byte of the address
         */
        unsigned char getFirst() const noexcept;
        /**
         * @brief Gets the second byte of the address.
         * @return The second byte of the address
         */
        unsigned char getSecond() const noexcept;
        /**
         * @brief Gets the third byte of the address.
         * @return The third byte of the address
         */
        unsigned char getThird() const noexcept;
        /**
         * @brief Gets the fourth byte of the address.
         * @return The fourth byte of the address
         */
        unsigned char getFourth() const noexcept;
        /**
         * @brief Gets the address in network byte order.
         * @return The address in network byte order
         */
        unsigned long getNetworkByteOrder() const noexcept;
        /**
         * @brief Gets the string representation of the address.
         * @return The string representation of the address
         */
        std::string str() const noexcept;

    private:
        unsigned char m_first;
        unsigned char m_second;
        unsigned char m_third;
        unsigned char m_fourth;
    };
}

#endif //IPV4ADDRESS_H
