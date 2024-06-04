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
        long getWidth();
        /**
         * @brief Sets the width of the window.
         * @param width The new width of the window
         */
        void setWidth(long width);
        /**
         * @brief Gets the height of the window.
         * @return The height of the window 
         */
        long getHeight();
        /**
         * @brief Sets the height of the window.
         * @param height The new height of the window
         */
        void setHeight(long height);
        /**
         * @brief Gets whether or not the window is maximized.
         * @return True if maximized, else false
         */
        bool isMaximized();
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
        bool apply(HWND hwnd);
#endif

    private:
        long m_width;
        long m_height;
        bool m_isMaximized;
    };
}

#endif //WINDOWGEOMETRY_H