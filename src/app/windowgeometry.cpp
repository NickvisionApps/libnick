#include "app/windowgeometry.h"

namespace Nickvision::App
{
    WindowGeometry::WindowGeometry()
        : m_width{ 800 },
        m_height{ 600 },
        m_isMaximized{ false }
    {

    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized)
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized }
    {

    }

#ifdef _WIN32
    WindowGeometry::WindowGeometry(HWND hwnd)
    {
        WINDOWPLACEMENT placement;
        GetWindowPlacement(hwnd, &placement);
        m_width = placement.rcNormalPosition.right - placement.rcNormalPosition.left;
        m_height = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top;
        m_isMaximized = placement.showCmd == SW_SHOWMAXIMIZED;
    }
#endif

    long WindowGeometry::getWidth() const
    {
        return m_width;
    }

    void WindowGeometry::setWidth(long width)
    {
        m_width = width;
    }

    long WindowGeometry::getHeight() const
    {
        return m_height;
    }

    void WindowGeometry::setHeight(long height)
    {
        m_height = height;
    }

    bool WindowGeometry::isMaximized() const
    {
        return m_isMaximized;
    }

    void WindowGeometry::setIsMaximized(bool isMaximized)
    {
        m_isMaximized = isMaximized;
    }

#ifdef _WIN32
    bool WindowGeometry::apply(HWND hwnd) const
    {
        WINDOWPLACEMENT placement;
        GetWindowPlacement(hwnd, &placement);
        placement.rcNormalPosition.right = placement.rcNormalPosition.left + m_width;
        placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + m_height;
        placement.showCmd = m_isMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
        return SetWindowPlacement(hwnd, &placement);
    }
#endif
}