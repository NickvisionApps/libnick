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
 * A model of a window's geometry.
 */

#ifndef WINDOWGEOMETRY_H
#define WINDOWGEOMETRY_H

#ifdef _WIN32
#include <windows.h>
#endif

namespace Nickvision::App
{
    /**
     * @brief A model of a window's geometry. 
     */
    class WindowGeometry
    {
    public:
        /**
         * @brief Construct a WindowGeometry.
         */
        WindowGeometry();
        /**
         * @brief Construct a WindowGeometry.
         * @param width The width of the window
         * @param height The height of the window
         * @param isMaximized Whether or not the window is maximized
         */
        WindowGeometry(long width, long height, bool isMaximized);
#ifdef _WIN32
        /**
         * @brief Construct a WindowGeometry.
         * @param hwnd The window handle to get the geometry from
         */
        WindowGeometry(HWND hwnd);
#endif
        /**
         * @brief Gets the width of the window.
         * @return The width of the window 
         */
        long getWidth() const;
        /**
         * @brief Sets the width of the window.
         * @param width The new width of the window
         */
        void setWidth(long width);
        /**
         * @brief Gets the height of the window.
         * @return The height of the window 
         */
        long getHeight() const;
        /**
         * @brief Sets the height of the window.
         * @param height The new height of the window
         */
        void setHeight(long height);
        /**
         * @brief Gets whether or not the window is maximized.
         * @return True if maximized, else false
         */
        bool isMaximized() const;
        /**
         * @brief Sets whether or not the window is maximized.
         * @param isMaximized True if maximized, else false
         */
        void setIsMaximized(bool isMaximized);
#ifdef _WIN32
        /**
         * @brief Applies the geometry to a window.
         * @param hwnd The window handle to apply the geometry to
         * @return True if successful, else false
         */
        bool apply(HWND hwnd) const;
#endif

    private:
        long m_width;
        long m_height;
        bool m_isMaximized;
    };
}

#endif //WINDOWGEOMETRY_H