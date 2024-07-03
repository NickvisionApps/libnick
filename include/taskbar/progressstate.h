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
 * States of progress on a taskbar button.
 */

#ifndef PROGRESSSTATE_H
#define PROGRESSSTATE_H

namespace Nickvision::Taskbar
{
    /**
     * @brief States of progress on a taskbar button.
     */
    enum class ProgressState
    {
        NoProgress = 0, ///< The taskbar item is in a no progress state.
        Indeterminate = 1, ///< The taskbar item is in an indeterminate progress state.
        Normal = 2, ///< The taskbar item is in a normal progress state.
        Error = 4, ///< The taskbar item is in an error progress state.
        Paused = 8 ///< The taskbar item is in a paused progress state.
    };
}

#endif //PROGRESSSTATE_H