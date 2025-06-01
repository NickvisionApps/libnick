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

#include <boost/json.hpp>

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
        /**
         * @brief Construct a WindowGeometry.
         * @param width The width of the window
         * @param height The height of the window
         * @param isMaximized Whether or not the window is maximized
         * @param x The x position of the window
         * @param y The y position of the window
         */
        WindowGeometry(long width, long height, bool isMaximized, long x, long y);
        /**
         * @brief Constructs a WindowGeometry.
         * @param json The json object
         */
        WindowGeometry(boost::json::object json);
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
        /**
         * @brief Gets the x position of the window.
         * @return The x position of the window
         */
        long getX() const;
        /**
         * @brief Sets the x position of the window.
         * @param x The new x position of the window
         */
        void setX(long x);
        /**
         * @brief Gets the y position of the window.
         * @return The y position of the window
         */
        long getY() const;
        /**
         * @brief Sets the y position of the window.
         * @param x The new y position of the window
         */
        void setY(long y);
        /**
         * @brief Converts the WindowGeometry to a json object.
         * @return The json representation of the WindowGeometry
         */
        boost::json::object toJson() const;

    private:
        long m_width;
        long m_height;
        bool m_isMaximized;
        long m_x;
        long m_y;
    };
}

#endif //WINDOWGEOMETRY_H
