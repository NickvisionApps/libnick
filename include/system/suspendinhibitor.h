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
 * An object to prevent the system from suspending.
 */

#ifndef SUSPENDINHIBITOR_H
#define SUSPENDINHIBITOR_H

#include <mutex>
#ifdef __APPLE__
#include <IOKit/pwr_mgt/IOPMLib.h>
#endif

namespace Nickvision::System
{
    /**
     * @brief An object to prevent the system from suspending.
     */
    class SuspendInhibitor
    {
    public:
        /**
         * @brief Constructs a SuspendInhibitor.
         * @brief This will not inhibit the system from suspending until the inhibit() function is called.
         */
        SuspendInhibitor();
        /**
         * @brief Copies a SuspendInhibitor object.
         * @param inhibitor The object to move
         */
        SuspendInhibitor(const SuspendInhibitor& inhibitor);
        /**
         * @brief Moves a SuspendInhibitor object.
         * @param inhibitor The object to move
         */
        SuspendInhibitor(SuspendInhibitor&& inhibitor) noexcept;
        /**
         * @brief Destructs a SuspendInhibitor.
         * @brief If the system is being inhibited, it will uninhibit.
         */
        ~SuspendInhibitor();
        /**
         * @brief Gets whether or not the system is being inhibited from suspending.
         * @return True if inhibiting, else false
         */
        bool isInhibiting() const;
        /**
         * @brief Inhibits the system from suspending.
         * @return True if successful, else false
         */
        bool inhibit();
        /**
         * @brief Uninhibits the system from suspending.
         * @return True if successful, else false
         */
        bool uninhibit();
        /**
         * @brief Copies a SuspendInhibitor object.
         * @param inhibitor The SuspendInhibitor to copy
         * @return this
         */
        SuspendInhibitor& operator=(const SuspendInhibitor& inhibitor);
        /**
         * @brief Moves a SuspendInhibitor object.
         * @param inhibitor The SuspendInhibitor to move
         * @return this
         */
        SuspendInhibitor& operator=(SuspendInhibitor&& inhibitor) noexcept;

    private:
        mutable std::mutex m_mutex;
        bool m_inhibiting;
#ifdef __linux__
        unsigned int m_cookie;
#elif defined(__APPLE__)
        IOPMAssertionID m_cookie;
#endif
    };
}

#endif //SUSPENDINHIBITOR_H