#include "app/windowgeometry.h"

namespace Nickvision::App
{
    WindowGeometry::WindowGeometry()
        : m_width{ 800 },
        m_height{ 600 },
        m_isMaximized{ false }
    {
#ifdef _WIN32
        m_x = 10;
        m_y = 10;
#endif
    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized)
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized }
    {
#ifdef _WIN32
        m_x = 10;
        m_y = 10;
#endif
    }

#ifdef _WIN32
    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized, long x, long y)
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized },
        m_x{ x },
        m_y{ y }
    {

    }

    WindowGeometry::WindowGeometry(HWND hwnd)
    {
        WINDOWPLACEMENT placement;
        GetWindowPlacement(hwnd, &placement);
        m_width = placement.rcNormalPosition.right - placement.rcNormalPosition.left;
        m_height = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top;
        m_isMaximized = placement.showCmd == SW_SHOWMAXIMIZED;
        m_x = placement.rcNormalPosition.left;
        m_y = placement.rcNormalPosition.top;
    }
#endif

    WindowGeometry::WindowGeometry(boost::json::object json)
        : m_width{ json["Width"].is_int64() ? static_cast<long>(json["Width"].as_int64()) : 0 },
        m_height{ json["Height"].is_int64() ? static_cast<long>(json["Height"].as_int64()) : 0 },
        m_isMaximized{ json["IsMaximized"].is_bool() ? json["IsMaximized"].as_bool() : false }
    {
#ifdef _WIN32
        m_x = json["X"].is_int64() ? static_cast<long>(json["X"].as_int64()) : 0;
        m_y = json["Y"].is_int64() ? static_cast<long>(json["Y"].as_int64()) : 0;
#endif
    }

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
        placement.rcNormalPosition.left = m_x;
        placement.rcNormalPosition.top = m_y;
        placement.rcNormalPosition.right = placement.rcNormalPosition.left + m_width;
        placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + m_height;
        placement.showCmd = m_isMaximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
        return SetWindowPlacement(hwnd, &placement);
    }
#endif

    boost::json::object WindowGeometry::toJson() const
    {
        boost::json::object json;
        json["Width"] = m_width;
        json["Height"] = m_height;
        json["IsMaximized"] = m_isMaximized;
#ifdef _WIN32
        json["X"] = m_x;
        json["Y"] = m_y;
#endif
        return json;
    }
}
