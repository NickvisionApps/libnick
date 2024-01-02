#ifdef _WIN32
#ifndef NOTIFYICON_H
#define NOTIFYICON_H

#define _CRT_SECURE_NO_WARNINGS
#define WM_NOTIFYICON_EVENT (WM_APP + 100)
#define IDM_CONTEXT_MENU (WM_APP + 200)

#include <filesystem>
#include <map>
#include <windows.h>
#include <shellapi.h>
#include "notifyiconmenu.h"
#include "shellnotificationsenteventargs.h"

namespace Nickvision::Aura::Notifications
{
	/**
	 * @brief An icon for the system tray.
	 * @brief This API is only available on Windows.
	 */
	class NotifyIcon
	{	
	public:
		/**
		 * @brief Constructs a NotifyIcon.
		 * @param hwnd The HWND handle of the main application window
		 * @param menu The model for the context menu of the NotifyIcon
		 * @param hidden Whether or not the NotifyIcon should be hidden by default
		 * @throw std::logic_error Thrown if Aura::init() was not called yet 
		 * @throw std::runtime_error Thrown if unable to create the NotifyIcon
		 */
		NotifyIcon(HWND hwnd, const NotifyIconMenu& contextMenu = { }, bool hidden = false);
		/**
		 * @brief Destructs a NotifyIcon. 
		 */
		~NotifyIcon() noexcept;
		/**
		 * @brief Hides the NotifyIcon.
		 * @return True if NotifyIcon was updated, else false
		 */
		bool hide() noexcept;
		/**
		 * @brief Shows the NotifyIcon.
		 * @return True if NotifyIcon was updated, else false
		 */
		bool show() noexcept;
		/**
		 * @brief Gets the tooltip being shown by the NotifyIcon.
		 * @return The tooltip being shown
		 */
		const std::string& getTooltip() const noexcept;
		/**
		 * @brief Sets the tooltip of the NotifyIcon.
		 * @param tooltip The toolip
		 * @return True if NotifyIcon was updated, else false
		 */
		bool setTooltip(const std::string& tooltip) noexcept;
		/**
		 * @brief Shows a notification from the NotifyIcon.
		 * @brief Supports the action "open" with action param being a path of a file or folder to open.
		 * @param e ShellNotificationSentEventArgs
		 * @return True if NotifyIcon was updated, else false
		 */
		bool notify(const ShellNotificationSentEventArgs& e) noexcept;

	private:
		/**
		 * @brief Gets a basic NOTIFYICONDATAA struct for this NotifyIcon.
		 * @return NOTIFYICONDATAA
		 */
		NOTIFYICONDATAA getBaseNotifyIconData() noexcept;
		/**
		 * @brief Handles a WM_NOTIFYICON_EVENT message.
		 * @param wParam WPARAM
		 * @param lParam LPARAM
		 * @return LRESULT
		 */
		LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
		std::string m_className;
		bool m_isHidden;
		std::string m_tooltip;
		NotifyIconMenu m_contextMenu;
		HWND m_hwnd;
		GUID m_guid;
		HMENU m_hmenu;
		std::filesystem::path m_openPath;

	private:
		static std::map<HWND, NotifyIcon*> m_icons;
		/**
		 * @brief The window procedure for NotifyIcons
		 * @param hwnd HWND
		 * @param uMsg UINT
		 * @param wParam WPARAM
		 * @param lParam LPARAM
		 */
		static LRESULT notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	};
}

#endif //NOTIFYICON_H
#endif