#include "app/windowgeometry.h"

namespace Nickvision::App
{
    WindowGeometry::WindowGeometry()
        : m_width{ 800 },
        m_height{ 600 },
        m_isMaximized{ false },
        m_x{ 10 },
        m_y{ 10 }
    {

    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized)
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized },
        m_x{ 10 },
        m_y{ 10 }
    {

    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized, long x, long y)
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized },
        m_x{ x },
        m_y{ y }
    {

    }

    WindowGeometry::WindowGeometry(boost::json::object json)
        : m_width{ json["Width"].is_int64() ? static_cast<long>(json["Width"].as_int64()) : 800 },
        m_height{ json["Height"].is_int64() ? static_cast<long>(json["Height"].as_int64()) : 600 },
        m_isMaximized{ json["IsMaximized"].is_bool() ? json["IsMaximized"].as_bool() : false },
        m_x{ json["X"].is_int64() ? static_cast<long>(json["X"].as_int64()) : 10 },
        m_y{ json["Y"].is_int64() ? static_cast<long>(json["Y"].as_int64()) : 10 }
    {

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

    long WindowGeometry::getX() const
    {
        return m_x;
    }

    void WindowGeometry::setX(long x)
    {
        m_x = x;
    }

    long WindowGeometry::getY() const
    {
        return m_y;
    }

    void WindowGeometry::setY(long y)
    {
        m_y = y;
    }

    boost::json::object WindowGeometry::toJson() const
    {
        boost::json::object json;
        json["Width"] = m_width;
        json["Height"] = m_height;
        json["IsMaximized"] = m_isMaximized;
        json["X"] = m_x;
        json["Y"] = m_y;
        return json;
    }
}
