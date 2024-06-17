#include "system/suspendinhibitor.h"
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <gio/gio.h>
#endif

namespace Nickvision::System
{
    SuspendInhibitor::SuspendInhibitor()
        : m_inhibiting{ false }
#ifdef __linux__
        , m_cookie{ 0 }
#endif
    {
    
    }

    SuspendInhibitor::SuspendInhibitor(const SuspendInhibitor& inhibitor)
    {
        std::lock_guard<std::mutex> lock{ inhibitor.m_mutex };
        m_inhibiting = inhibitor.m_inhibiting;
#ifdef __linux__
        m_cookie = inhibitor.m_cookie;
#endif
    }

    SuspendInhibitor::SuspendInhibitor(SuspendInhibitor&& inhibitor) noexcept
    {
        std::lock_guard<std::mutex> lock{ inhibitor.m_mutex };
        m_inhibiting = std::move(inhibitor.m_inhibiting);
#ifdef __linux__
        m_cookie = std::move(inhibitor.m_cookie);
#endif
    }

    SuspendInhibitor::~SuspendInhibitor()
    {
        if(m_inhibiting)
        {
            uninhibit();
        }
    }

    bool SuspendInhibitor::isInhibiting() const
    {
        return m_inhibiting;
    }

    bool SuspendInhibitor::inhibit()
    {
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
        GDBusProxy* proxy{ g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr, "org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver.Inhibit", nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            return false;
        }
        GVariant* result{ g_dbus_proxy_call_sync(proxy, "Inhibit", g_variant_new("(ss)", "Nickvision", "Preventing suspend"), G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error) };
        if(error)
        {
            g_error_free(error);
            g_object_unref(proxy);
            return false;
        }
        m_cookie = g_variant_get_uint32(result);
        g_variant_unref(result);
        g_object_unref(proxy);
#endif
        m_inhibiting = true;
        return true;
    }

    bool SuspendInhibitor::uninhibit()
    {
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
        GDBusProxy* proxy{ g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE, nullptr, "org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver.UnInhibit", nullptr, &error) };
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
#endif
        m_inhibiting = false;
        return true;
    }

    SuspendInhibitor& SuspendInhibitor::operator=(const SuspendInhibitor& inhibitor)
    {
        if (this != &inhibitor)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ inhibitor.m_mutex };
            m_inhibiting = inhibitor.m_inhibiting;
#ifdef __linux__
            m_cookie = inhibitor.m_cookie;
#endif
        }
        return *this;
    }

    SuspendInhibitor& SuspendInhibitor::operator=(SuspendInhibitor&& inhibitor) noexcept
    {
        if (this != &inhibitor)
        {
            std::lock_guard<std::mutex> lock{ m_mutex };
            std::lock_guard<std::mutex> lock2{ inhibitor.m_mutex };
            m_inhibiting = std::move(inhibitor.m_inhibiting);
#ifdef __linux__
            m_cookie = std::move(inhibitor.m_cookie);
#endif
        }
        return *this;
    }
}