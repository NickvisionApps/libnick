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
 * An event argument that contains a single parameter.
 */

#ifndef PARAMEVENTARGS_H
#define PARAMEVENTARGS_H

#include <type_traits>
#include "eventargs.h"

namespace Nickvision::Events
{
    /**
     * @brief An event argument that contains a single parameter.
     * @brief T must be copy and move constructible.
     */
    template<typename T>
    class ParamEventArgs : public EventArgs
    {
    static_assert(std::is_copy_constructible_v<T> == true);
    static_assert(std::is_move_constructible_v<T> == true);

    public:
        /**
         * @brief Constructs a ParamEventArgs.
         * @param param The parameter to store in the event args
         */
        ParamEventArgs(const T& param)
            : m_param{ param }
        {

        }
        /**
         * @brief Gets the param stored in the event args.
         * @return The param stored
         */
        const T& getParam() const
        {
            return m_param;
        }

    private:
        T m_param;
    };
}

#endif //PARAMEVENTARGS_H