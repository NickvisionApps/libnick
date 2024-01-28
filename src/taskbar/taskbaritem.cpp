#include "taskbar/taskbaritem.h"
#include <limits>
#ifdef _WIN32
#include <dwmapi.h>
using namespace Gdiplus;
#elif defined(__linux__)
#include <stdlib.h>
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
		if (m_connection)
		{
			GDBusMessage* message{ g_dbus_message_new_signal(m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update") };
			GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_new_dict_entry(g_variant_new_string("progress-visible"), g_variant_new_boolean(m_progressState >= ProgressState::Normal)) };
			GVariant* tuple{ g_variant_new_tuple(params, 2) };
			g_dbus_message_set_body(message, tuple);
			g_dbus_connection_send_message(m_connection, message, G_DBUS_SEND_MESSAGE_FLAGS_NONE, nullptr, nullptr);
			g_object_unref(G_OBJECT(message));
		}
#endif
	}

	double TaskbarItem::getProgress() const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_progress;
	}

	void TaskbarItem::setProgress(double progress)
	{
		std::unique_lock<std::mutex> lock{ m_mutex };
		m_progress = progress;
#ifdef _WIN32
		if (m_taskbar)
		{
			m_taskbar->SetProgressValue(m_hwnd, static_cast<unsigned long>(m_progress * 100), 100u);
		}
#elif defined(__linux__)
		if (m_connection)
		{
			GDBusMessage* message{ g_dbus_message_new_signal(m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update") };
			GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_new_dict_entry(g_variant_new_string("progress"), g_variant_new_double(m_progress)) };
			GVariant* tuple{ g_variant_new_tuple(params, 2) };
			g_dbus_message_set_body(message, tuple);
			g_dbus_connection_send_message(m_connection, message, G_DBUS_SEND_MESSAGE_FLAGS_NONE, nullptr, nullptr);
			g_object_unref(G_OBJECT(message));
		}
#endif
		lock.unlock();
		setProgressState(progress == 0 ? ProgressState::NoProgress : ProgressState::Normal);
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
		if (m_connection)
		{
			GDBusMessage* message{ g_dbus_message_new_signal(m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update") };
			GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_new_dict_entry(g_variant_new_string("urgent"), g_variant_new_boolean(m_urgent)) };
			GVariant* tuple{ g_variant_new_tuple(params, 2) };
			g_dbus_message_set_body(message, tuple);
			g_dbus_connection_send_message(m_connection, message, G_DBUS_SEND_MESSAGE_FLAGS_NONE, nullptr, nullptr);
			g_object_unref(G_OBJECT(message));
		}
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
				DWORD accentColor;
				BOOL opaue{ FALSE };
				Graphics windowGraphics{ m_hwnd };
				SolidBrush background{ DwmGetColorizationColor(&accentColor, &opaue) == S_OK ? Color::Color(accentColor) : Color::Color(0, 0, 0) };
				SolidBrush foreground{ Color::Color(255, 255, 255) };
				Bitmap bitmap{ 16, 16, &windowGraphics };
				Graphics graphics{ &bitmap };
				FontFamily fontFamily{ L"Microsoft Sans Serif" };
				Font font{ &fontFamily, m_count <= 99 ? (m_count < 10 ? 9.0f : 7.5f) : 7.0f };
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
		}
#elif defined(__linux__)
		if (m_connection)
		{
			GDBusMessage* message{ g_dbus_message_new_signal(m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update") };
			GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_new_dict_entry(g_variant_new_string("count-visible"), g_variant_new_boolean(m_countVisible)) };
			GVariant* tuple{ g_variant_new_tuple(params, 2) };
			g_dbus_message_set_body(message, tuple);
			g_dbus_connection_send_message(m_connection, message, G_DBUS_SEND_MESSAGE_FLAGS_NONE, nullptr, nullptr);
			g_object_unref(G_OBJECT(message));
		}
#endif
	}

	long TaskbarItem::getCount() const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_count;
	}

	void TaskbarItem::setCount(long count)
	{
		std::unique_lock<std::mutex> lock{ m_mutex };
		m_count = count;
#ifdef __linux__
		if (m_connection)
		{
			GDBusMessage* message{ g_dbus_message_new_signal(m_objectPath.c_str(), "com.canonical.Unity.LauncherEntry", "Update") };
			GVariant* params[2]{ g_variant_new_string(m_appUri.c_str()), g_variant_new_dict_entry(g_variant_new_string("count"), g_variant_new_int64(m_count)) };
			GVariant* tuple{ g_variant_new_tuple(params, 2) };
			g_dbus_message_set_body(message, tuple);
			g_dbus_connection_send_message(m_connection, message, G_DBUS_SEND_MESSAGE_FLAGS_NONE, nullptr, nullptr);
			g_object_unref(G_OBJECT(message));
		}
#endif
		lock.unlock();
		setCountVisible(count > 0);
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
#endif
}