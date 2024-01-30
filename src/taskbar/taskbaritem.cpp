#include "taskbar/taskbaritem.h"
#include <cstdlib>
#include <limits>
#ifdef _WIN32
#include <dwmapi.h>
using namespace Gdiplus;
#endif

namespace Nickvision::Taskbar
{
    TaskbarItem::TaskbarItem()
        : m_progressState{ ProgressState::NoProgress },
        m_progress{ 0.0 },
        m_urgent{ false },
        m_countVisible{ false },
        m_count{ 0 }
    {
#ifdef _WIN32
        GdiplusStartupInput gdiStartupIn;
        GdiplusStartupOutput gdiStartupOut;
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        GdiplusStartup(&m_gdi, &gdiStartupIn, &gdiStartupOut);
        m_hwnd = nullptr;
        m_taskbar = nullptr;
#elif defined(__linux__)
        m_connection = nullptr;
        m_objectPath = "";
        m_appUri = "";
#endif
    }

    TaskbarItem::~TaskbarItem()
    {
        setProgressState(ProgressState::NoProgress);
        setUrgent(false);
        setCountVisible(false);
#ifdef _WIN32
        GdiplusShutdown(m_gdi);    
#endif
    }

    ProgressState TaskbarItem::getProgressState() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_progressState;
    }

    void TaskbarItem::setProgressState(ProgressState state)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_progressState = state;
#ifdef _WIN32
        if (m_taskbar)
        {
            m_taskbar->SetProgressState(m_hwnd, (TBPFLAG)m_progressState);
        }
#elif defined(__linux__)
        sendDBusUpdate();
#endif
    }

    double TaskbarItem::getProgress() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_progress;
    }

    void TaskbarItem::setProgress(double progress)
    {
        setProgressState(progress == 0 ? ProgressState::NoProgress : ProgressState::Normal);
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_progress = progress;
#ifdef _WIN32
        if (m_taskbar)
        {
            m_taskbar->SetProgressValue(m_hwnd, static_cast<unsigned long>(m_progress * 100), 100u);
        }
#elif defined(__linux__)
        sendDBusUpdate();
#endif
    }

    bool TaskbarItem::getUrgent() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_urgent;
    }

    void TaskbarItem::setUrgent(bool urgent)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_urgent = urgent;
#ifdef _WIN32
        if (m_taskbar)
        {
            FLASHWINFO flashInfo;
            flashInfo.cbSize = sizeof(FLASHWINFO);
            flashInfo.hwnd = m_hwnd;
            flashInfo.dwFlags = m_urgent ? (FLASHW_TRAY | FLASHW_TIMER) : FLASHW_STOP;
            flashInfo.uCount = (std::numeric_limits<unsigned>::max)();
            flashInfo.dwTimeout = 0;
            FlashWindowEx(&flashInfo);
        }
#elif defined(__linux__)
        sendDBusUpdate();
#endif
    }

    bool TaskbarItem::getCountVisible() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_countVisible;
    }

    void TaskbarItem::setCountVisible(bool countVisible)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_countVisible = countVisible;
#ifdef _WIN32
        if (m_taskbar)
        {
            if (!m_countVisible)
            {
                m_taskbar->SetOverlayIcon(m_hwnd, nullptr, L"");
            }
            else
            {
                drawCountIcon();
            }
        }
#elif defined(__linux__)
        sendDBusUpdate();
#endif
    }

    long TaskbarItem::getCount() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_count;
    }

    void TaskbarItem::setCount(long count)
    {
        setCountVisible(count > 0);
        std::lock_guard<std::mutex> lock{ m_mutex };
        m_count = count;
#ifdef _WIN32
        drawCountIcon();
#elif defined(__linux__)
        sendDBusUpdate();
#endif
    }

#ifdef _WIN32
    bool TaskbarItem::connect(HWND hwnd)
    {
        if(m_hwnd)
        {
            return true;
        }
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (!hwnd)
        {
            return false;
        }
        if (CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL, __uuidof(ITaskbarList3), (LPVOID*)&m_taskbar) == S_OK)
        {
            m_hwnd = hwnd;
            return m_taskbar->HrInit() == S_OK;
        }
        return false;
    }

    void TaskbarItem::drawCountIcon()
    {
        DWORD accentColor;
        Graphics windowGraphics{ m_hwnd };
        SolidBrush background{ DwmGetColorizationColor(&accentColor, nullptr) == S_OK ? Color::Color(accentColor) : Color::Color(0, 0, 0) };
        SolidBrush foreground{ Color::Color(255, 255, 255) };
        Color backgroundColor;
        background.GetColor(&backgroundColor);
        background.SetColor({ 255, backgroundColor.GetR(), backgroundColor.GetG(), backgroundColor.GetB() });
        Bitmap bitmap{ 16, 16, &windowGraphics };
        Graphics graphics{ &bitmap };
        FontFamily fontFamily{ L"Microsoft Sans Serif" };
        Font font{ &fontFamily, m_count <= 99 ? (m_count < 10 ? 8.0f : 7.5f) : 7.0f };
        std::wstring countStr{ m_count > 99 ? L"99+" : std::to_wstring(m_count) };
        SizeF stringSize;
        graphics.MeasureString(countStr.c_str(), (int)countStr.length(), &font, SizeF(16, 16), StringFormat::GenericDefault(), &stringSize);
        graphics.FillEllipse(&background, Rect(0, 0, 16, 16));
        graphics.DrawString(countStr.c_str(), (int)countStr.length(), &font, PointF((16 - stringSize.Width) / 2, (16 - stringSize.Height) / 2), &foreground);
        HICON icon{ nullptr };
        bitmap.GetHICON(&icon);
        m_taskbar->SetOverlayIcon(m_hwnd, icon, std::to_wstring(m_count).c_str());
        DestroyIcon(icon);
    }
#elif defined(__linux__)
    bool TaskbarItem::connect(const std::string& desktopFile)
    {
        if(m_connection)
        {
            return true;
        }
        std::lock_guard<std::mutex> lock{ m_mutex };
        if (desktopFile.empty())
        {
            return false;
        }
        m_connection = g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr); //returns a singleton, no need to manage it
        if (m_connection)
        {
            unsigned long hash{ 5381 };
            for(char c : desktopFile)
            {
                hash = (hash << 5) + hash + c;
            }
            m_objectPath = "/com/canonical/unity/launcherentry/" + std::to_string(hash);
            m_appUri = "application://" + desktopFile;
            return true;
        }
        return false;
    }

    void TaskbarItem::sendDBusUpdate()
    {
        if(m_connection)
        {
            GVariantBuilder builder;
            g_variant_builder_init(&builder, G_VARIANT_TYPE("a{sv}"));
            g_variant_builder_add(&builder, "{sv}", "progress-visible", g_variant_new_boolean(m_progressState >= ProgressState::Normal));
            g_variant_builder_add(&builder, "{sv}", "progress", g_variant_new_double(m_progress));
            g_variant_builder_add(&builder, "{sv}", "urgent", g_variant_new_boolean(m_urgent));
            g_variant_builder_add(&builder, "{sv}", "count-visible", g_variant_new_boolean(m_countVisible));
            g_variant_builder_add(&builder, "{sv}", "count", g_variant_new_int64(m_count));
            GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_builder_end(&builder) };
            GVariant* tuple{ g_variant_new_tuple(params, 2) };
            g_dbus_connection_emit_signal(m_connection, nullptr, m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update", tuple, nullptr);
        }
    }
#endif
}