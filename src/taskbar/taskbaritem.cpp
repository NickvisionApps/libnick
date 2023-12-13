#include "taskbar/taskbaritem.h"
#include <memory>
#include <string>
#ifdef _WIN32
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
using namespace Gdiplus;
#endif

namespace Nickvision::Aura::Taskbar
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
		CoInitialize(nullptr);
		GdiplusStartup(&m_gdi, &gdiStartupIn, &gdiStartupOut);
		m_hwnd = nullptr;
		m_taskbar = nullptr;
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
		return m_progressState;
	}

	void TaskbarItem::setProgressState(ProgressState state)
	{
		m_progressState = state;
#ifdef _WIN32
		if (m_taskbar)
		{
			m_taskbar->SetProgressState(m_hwnd, (TBPFLAG)m_progressState);
		}
#endif
	}

	double TaskbarItem::getProgress() const
	{
		return m_progress;
	}

	void TaskbarItem::setProgress(double progress)
	{
		m_progress = progress;
#ifdef _WIN32
		if (m_taskbar)
		{
			m_taskbar->SetProgressValue(m_hwnd, static_cast<unsigned long>(progress * 100), 100u);
		}
#endif
		setProgressState(ProgressState::Normal);
	}

	bool TaskbarItem::getUrgent() const
	{
		return m_urgent;
	}

	void TaskbarItem::setUrgent(bool urgent)
	{
		m_urgent = urgent;
#ifdef _WIN32
		if (m_taskbar)
		{
			FLASHWINFO flashInfo;
			flashInfo.cbSize = sizeof(FLASHWINFO);
			flashInfo.hwnd = m_hwnd;
			flashInfo.dwFlags = urgent ? (FLASHW_TRAY | FLASHW_TIMER) : FLASHW_STOP;
			flashInfo.uCount = UINT_MAX;
			flashInfo.dwTimeout = 0;
			FlashWindowEx(&flashInfo);
		}
#endif
	}

	bool TaskbarItem::getCountVisible() const
	{
		return m_countVisible;
	}

	void TaskbarItem::setCountVisible(bool countVisible)
	{
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
#endif
	}

	long TaskbarItem::getCount() const
	{
		return m_count;
	}

	void TaskbarItem::setCount(long count)
	{
		m_count = count;
		setCountVisible(count >= 0);
	}

#ifdef _WIN32
	bool TaskbarItem::connect(HWND hwnd)
	{
		if (!hwnd)
		{
			return false;
		}
		if (CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL, __uuidof(ITaskbarList3), (LPVOID*)&m_taskbar) == S_OK)
		{
			m_hwnd = hwnd;
			m_taskbar->HrInit();
			return true;
		}
		return false;
	}
#endif
}