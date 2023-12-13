#include "taskbar/taskbaritem.h"

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
		CoInitialize(nullptr);
		m_hwnd = nullptr;
		m_background = nullptr;
		m_foreground = nullptr;
		m_taskbar = nullptr;
#endif
	}

	TaskbarItem::~TaskbarItem()
	{
		setCountVisible(false);
		setProgressState(ProgressState::NoProgress);
		setUrgent(false);
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
			m_taskbar->SetProgressState(m_hwnd, (TBPFLAG)m_progressState) == S_OK;
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
			m_taskbar->SetProgressValue(m_hwnd, static_cast<unsigned long>(progress * 100), 100u) == S_OK;
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
			if (!countVisible)
			{
				m_taskbar->SetOverlayIcon(m_hwnd, nullptr, L"") == S_OK;
			}
			else
			{
					
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
	bool TaskbarItem::connect(HWND hwnd, HBRUSH background, HBRUSH foreground)
	{
		if (!hwnd)
		{
			return false;
		}
		if (CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_ALL, __uuidof(ITaskbarList3), (LPVOID*)&m_taskbar) == S_OK)
		{
			m_hwnd = hwnd;
			m_background = background;
			m_foreground = foreground;
			m_taskbar->HrInit();
			return true;
		}
		return false;
	}
#endif
}