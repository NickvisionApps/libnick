#ifndef TASKBARITEM_H
#define TASKBARITEM_H

#include <memory>
#include <mutex>
#include <string>
#include "progressstate.h"
#ifdef _WIN32
#include <windows.h>
#include <atlbase.h>
#include <gdiplus.h>
#include <shlobj_core.h>
#pragma comment(lib,"gdiplus.lib")
#elif defined(__linux__)
#include <glib.h>
#include <gio/gio.h>
#endif

namespace Nickvision::Aura::Taskbar
{
	/**
	 * @brief An item on the taskbar. 
	 */
	class TaskbarItem
	{
	public:
		/**
		 * @brief Constructs a TaskbarItem. 
		 */
		TaskbarItem() noexcept;
		/**
		 * @brief Destructs a TaskbarItem.
		 */
		~TaskbarItem() noexcept;
		/**
		 * @brieg Gets the state of the progress.
		 * @return ProgressState
		 */
		ProgressState getProgressState() const noexcept;
		/**
		 * @brief Sets the state of the progress.
		 * @param state The new ProgressState
		 */
		void setProgressState(ProgressState state) noexcept;
		/**
		 * @brief Gets the value of the progress.
		 * @return The progress value
		 */
		double getProgress() const noexcept;
		/**
		 * @brief Sets the value of the progress. Setting the progress value will set the progress state to Normal if progress > 0, else will set progress state to NoProgress.
		 * @brief Should be a value between 0 and 1.
		 * @param progress The new progress value
		 */
		void setProgress(double progress) noexcept;
		/**
		 * @brief Gets whether or not the taskbar item is shown in an urgent state.
		 * @return True if in urgent state, else false
		 */
		bool getUrgent() const noexcept;
		/**
		 * @brief Sets whether or not the taskbar item is shown in an urgent state.
		 * @param urgent True for urgent state, else false
		 */
		void setUrgent(bool urgent) noexcept;
		/**
		 * @brief Gets whether or not the count is visible on the taskbar item.
		 * @return True if count visible, else false
		 */
		bool getCountVisible() const noexcept;
		/**
		 * @brief Sets whether or not the count is visible on the taskbar item.
		 * @param countVisible True for visible count, else false
		 */
		void setCountVisible(bool countVisible) noexcept;
		/**
		 * @brief Gets the count shown on the taskbar item.
		 * @return The count value
		 */
		long getCount() const noexcept;
		/**
		 * @brief Sets the count shown on the taskbar item. Setting the count will set the count visible to true if count > 0, else will set count visible to false.
		 * @param count The new count value
		 */
		void setCount(long count) noexcept;
#ifdef _WIN32
		/**
		 * @brief Connects a taskbar item to the application.
		 * @param hwnd The HWND of the main application window
		 * @return True if connection successful, else false
		 */
		bool connect(HWND hwnd) noexcept;
#elif defined(__linux__)
		/**
		 * @brief Connects a taskbar item to the application.
		 * @param desktopFile The desktop file name with the file extension of the running application
		 * @return True if connection successful, else false
		 */
		bool connect(const std::string& desktopFile) noexcept;
#endif

	private:
		mutable std::mutex m_mutex;
		ProgressState m_progressState;
		double m_progress;
		bool m_urgent;
		bool m_countVisible;
		long m_count;
#ifdef _WIN32
		HWND m_hwnd;
		CComPtr<ITaskbarList3> m_taskbar;
		ULONG_PTR m_gdi;
#elif defined(__linux__)
		std::shared_ptr<GDBusConnection> m_connection;
		std::string m_appUri;
#endif
	};
}

#endif //TASKBARITEM_H