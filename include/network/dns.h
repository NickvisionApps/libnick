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
 * Functions for working with DNS.
 */

#ifndef DNS_H
#define DNS_H

#include <string>
#include <vector>
#include "ipv4address.h"

namespace Nickvision::Network::DNS
{
    /**
     * @brief Resolves a hostname to IPv4 addresses.
     * @param hostname The hostname to resolve
     * @return A list of IPv4 addresses that the hostname resolves to
     */
    std::vector<IPv4Address> a(const std::string& hostname);
    /**
     * @brief Gets text records for a hostname.
     * @param hostname The hostname to resolve
     * @return A list of text records for the hostname
     */
    std::vector<std::string> txt(const std::string& hostname);
}

#endif //DNS_H