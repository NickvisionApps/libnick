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
 * Functions for working with the system's hardware information.
 */

#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

/**
 * @brief Functions for working with the system's hardware information.
 */
namespace Nickvision::System::HardwareInfo
{
    /**
     * @brief Gets the number of logical processors (CPU) on the system.
     * @return The number of logical processors
     */
    unsigned int getNumberOfProcessors() noexcept;
    /**
     * @brief Gets the amount of RAM in the system in bytes.
     * @return The amount of RAM in bytes
     */
    unsigned long long getTotalRamSize() noexcept;
    /**
     * @brief Gets the amount of RAM free in the system in bytes.
     * @return The amount of free RAM in bytes
     */
    unsigned long long getFreeRamSize() noexcept;
}

#endif //HARDWAREINFO_H
