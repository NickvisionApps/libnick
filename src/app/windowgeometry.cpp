#include "app/windowgeometry.h"

namespace Nickvision::App
{
    WindowGeometry::WindowGeometry() noexcept
        : m_width{ 800 },
        m_height{ 600 },
        m_isMaximized{ false },
        m_x{ 10 },
        m_y{ 10 }
    {

    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized) noexcept
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized },
        m_x{ 10 },
        m_y{ 10 }
    {

    }

    WindowGeometry::WindowGeometry(long width, long height, bool isMaximized, long x, long y) noexcept
        : m_width{ width },
        m_height{ height },
        m_isMaximized{ isMaximized },
        m_x{ x },
        m_y{ y }
    {

    }

    WindowGeometry::WindowGeometry(boost::json::object json) noexcept
        : m_width{ json["Width"].is_int64() ? static_cast<long>(json["Width"].as_int64()) : 800 },
        m_height{ json["Height"].is_int64() ? static_cast<long>(json["Height"].as_int64()) : 600 },
        m_isMaximized{ json["IsMaximized"].is_bool() ? json["IsMaximized"].as_bool() : false },
        m_x{ json["X"].is_int64() ? static_cast<long>(json["X"].as_int64()) : 10 },
        m_y{ json["Y"].is_int64() ? static_cast<long>(json["Y"].as_int64()) : 10 }
    {

    }

    long WindowGeometry::getWidth() const noexcept
    {
        return m_width;
    }

    void WindowGeometry::setWidth(long width) noexcept
    {
        m_width = width;
    }

    long WindowGeometry::getHeight() const noexcept
    {
        return m_height;
    }

    void WindowGeometry::setHeight(long height) noexcept
    {
        m_height = height;
    }

    bool WindowGeometry::isMaximized() const noexcept
    {
        return m_isMaximized;
    }

    void WindowGeometry::setIsMaximized(bool isMaximized) noexcept
    {
        m_isMaximized = isMaximized;
    }

    long WindowGeometry::getX() const noexcept
    {
        return m_x;
    }

    void WindowGeometry::setX(long x) noexcept
    {
        m_x = x;
    }

    long WindowGeometry::getY() const noexcept
    {
        return m_y;
    }

    void WindowGeometry::setY(long y) noexcept
    {
        m_y = y;
    }

    boost::json::value WindowGeometry::toJson() const noexcept
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
