#include "system/suspendinhibitor.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <gio/gio.h>
#elif defined(__APPLE__)
#include <AvailabilityMacros.h>
#endif

namespace Nickvision::System
{
    SuspendInhibitor::SuspendInhibitor() noexcept
        : m_inhibiting{ false }
#ifdef __linux__
        , m_cookie{ 0 }
#endif
    {
    
    }

    SuspendInhibitor::SuspendInhibitor(const SuspendInhibitor& other) noexcept
    {
        std::lock_guard<std::mutex> lock{ other.m_mutex };
        m_inhibiting = other.m_inhibiting;
#ifndef _WIN32
        m_cookie = other.m_cookie;
#endif
    }

    SuspendInhibitor::SuspendInhibitor(SuspendInhibitor&& other) noexcept
    {
        std::lock_guard<std::mutex> lock{ other.m_mutex };
        m_inhibiting = std::move(other.m_inhibiting);
#ifndef _WIN32
        m_cookie = std::move(other.m_cookie);
        other.m_cookie = 0;
#endif
        other.m_inhibiting = false;
    }

    SuspendInhibitor::~SuspendInhibitor() noexcept
    {
        if(m_inhibiting)
        {
            uninhibit();
        }
    }

    bool SuspendInhibitor::isInhibiting() const noexcept
    {
        return m_inhibiting;
    }

    bool SuspendInhibitor::inhibit() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_inhibiting)
        {
            return true;
        }
#ifdef _WIN32
        if(SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED) == NULL)
        {
            return false;
        }
#elif defined(__linux__)
        GError* error{ nullptr };
        GDBusProxy* proxy{ g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr, "org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver", nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            return false;
        }
        GVariant* result{ g_dbus_proxy_call_sync(proxy, "Inhibit", g_variant_new("(ss)", "org.nickvision.libnick", "Preventing suspend"), G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            g_object_unref(proxy);
            return false;
        }
        g_variant_get(result, "(u)", &m_cookie);
        g_variant_unref(result);
        g_object_unref(proxy);
#elif defined(__APPLE__)
    #if MAC_OS_X_VERSION_MIN_REQUIRED > 1060
        IOReturn result{ IOPMAssertionCreateWithName(kIOPMAssertionTypePreventUserIdleSystemSleep, kIOPMAssertionLevelOn, CFSTR("Nickvision preventing suspend"), &m_cookie) };
        if(result != kIOReturnSuccess)
        {
            return false;
        }
    #else
        return false;
    #endif
#endif
        m_inhibiting = true;
        return true;
    }

    bool SuspendInhibitor::uninhibit() noexcept
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_inhibiting)
        {
            return true;
        }
#ifdef _WIN32
        if(SetThreadExecutionState(ES_CONTINUOUS) == NULL)
        {
            return false;
        }
#elif defined(__linux__)
        GError* error{ nullptr };
        GDBusProxy* proxy{ g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr, "org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver", nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            return false;
        }
        GVariant* result{ g_dbus_proxy_call_sync(proxy, "UnInhibit", g_variant_new("(u)", m_cookie), G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            g_object_unref(proxy);
            return false;
        }
        g_variant_unref(result);
        g_object_unref(proxy);
#elif defined(__APPLE__)
        IOReturn result{ IOPMAssertionRelease(m_cookie) };
        if(result != kIOReturnSuccess)
        {
            return false;
        }
#endif
        m_inhibiting = false;
        return true;
    }

    SuspendInhibitor& SuspendInhibitor::operator=(const SuspendInhibitor& other)
    {
        if (this != &other)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ other.m_mutex };
            m_inhibiting = other.m_inhibiting;
#ifndef _WIN32
            m_cookie = other.m_cookie;
#endif
        }
        return *this;
    }

    SuspendInhibitor& SuspendInhibitor::operator=(SuspendInhibitor&& other) noexcept
    {
        if (this != &other)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ other.m_mutex };
            m_inhibiting = std::move(other.m_inhibiting);
#ifndef _WIN32
            m_cookie = std::move(other.m_cookie);
            other.m_cookie = 0;
#endif
            other.m_inhibiting = false;
        }
        return *this;
    }
}