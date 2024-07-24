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
 * A model of a MAC address.
 */

#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <optional>
#include <string>

namespace Nickvision::Network
{
    /**
     * @brief A model of a MAC address.
     */
    class MacAddress
    {
    public:
        /**
         * @brief Constructs a MacAddress.
         * @param oui1 The first byte of the OUI
         * @param oui2 The second byte of the OUI
         * @param oui3 The third byte of the OUI
         * @param nic1 The first byte of the NIC
         * @param nic2 The second byte of the NIC
         * @param nic3 The third byte of the NIC
         */
        MacAddress(unsigned char oui1, unsigned char oui2, unsigned char oui3, unsigned char nic1, unsigned char nic2, unsigned char nic3);
        /**
         * @brief Gets the first byte of the address.
         * @return The first byte of the address
         */
        unsigned char getFirst() const;
        /**
         * @brief Gets the second byte of the address.
         * @return The second byte of the address
         */
        unsigned char getSecond() const;
        /**
         * @brief Gets the third byte of the address.
         * @return The third byte of the address
         */
        unsigned char getThird() const;
        /**
         * @brief Gets the fourth byte of the address.
         * @return The fourth byte of the address
         */
        unsigned char getFourth() const;
        /**
         * @brief Gets the fifth byte of the address.
         * @return The fifth byte of the address
         */
        unsigned char getFifth() const;
        /**
         * @brief Gets the sixth byte of the address.
         * @return The sixth byte of the address
         */
        unsigned char getSixth() const;
        /**
         * @brief Gets the string representation of the address.
         * @return The string representation of the address
         */
        std::string str() const;
        /**
         * @brief Parses a MacAddress from a string.
         * @param address The string address to parse
         * @return The parsed address if successful, else std::nullopt
         */
        static std::optional<MacAddress> parse(const std::string& address);

    private:
        unsigned char m_oui1;
        unsigned char m_oui2;
        unsigned char m_oui3;
        unsigned char m_nic1;
        unsigned char m_nic2;
        unsigned char m_nic3;
    };
}

#endif //MACADDRESS_H