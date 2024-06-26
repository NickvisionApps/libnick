#ifdef _WIN32
#ifndef NOTIFYICON_H
#define NOTIFYICON_H

#define _CRT_SECURE_NO_WARNINGS
#define WM_NOTIFYICON_EVENT (WM_APP + 100)
#define IDM_CONTEXT_MENU (WM_APP + 200)

#include <filesystem>
#include <unordered_map>
#include <windows.h>
#include <shellapi.h>
#include "notifyiconmenu.h"
#include "shellnotificationsenteventargs.h"

namespace Nickvision::Notifications
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
         * @param tooltip The tooltip text of the icon
         * @param menu The model for the context menu of the NotifyIcon
         * @param hidden Whether or not the NotifyIcon should be hidden by default
         * @throw std::runtime_error Thrown if unable to create the NotifyIcon
         */
        NotifyIcon(HWND hwnd, const std::wstring& tooltip, const NotifyIconMenu& contextMenu = { }, bool hidden = false);
        /**
         * @brief Destructs a NotifyIcon. 
         */
        ~NotifyIcon();
        /**
         * @brief Hides the icon.
         * @return True if icon was hidden, else false
         */
        bool hide();
        /**
         * @brief Shows the icon.
         * @return True if icon was shown, else false
         */
        bool show();
        /**
         * @brief Gets the tooltip text of the icon.
         * @return The tooltip text
         */
        const std::wstring& getTooltip() const;
        /**
         * @brief Sets the tooltip text of the icon.
         * @param tooltip The toolip text
         * @return True if the tooltip was updated, else false
         */
        bool setTooltip(const std::wstring& tooltip);
        /**
         * @brief Gets the context menu of the icon.
         * @return The model for the context menu
         */
        const NotifyIconMenu& getContextMenu() const;
        /**
         * @brief Sets the context menu of the icon.
         * @param menu The model for the context menu
         * @return True if the context menu was updated, else false
         */
        bool setContextMenu(const NotifyIconMenu& menu);
        /**
         * @brief Shows a notification from the icon.
         * @brief Supports the action "open" with action param being a path of a file or folder to open.
         * @param e ShellNotificationSentEventArgs
         * @return True if notification was shown from the icon
         */
        bool notify(const ShellNotificationSentEventArgs& e);

    public:
        /**
         * @brief Gets a NotifyIcon from an HWND.
         * @param hwnd HWND
         * @return The NotifyIcon associated with the HWND if found, else nullptr
         */
        static NotifyIcon* getFromHWND(HWND hwnd);

    private:
        /**
         * @brief Gets a basic NOTIFYICONDATAW struct for this NotifyIcon.
         * @return NOTIFYICONDATAW
         */
        NOTIFYICONDATAW getBaseNotifyIconData();
        /**
         * @brief Creates the context menu for the NotifyIcon.
         * @return True if the context menu was created, else false
         */
        bool createContextMenu();
        /**
         * @brief Handles a WM_NOTIFYICON_EVENT message.
         * @param wParam WPARAM
         * @param lParam LPARAM
         * @return LRESULT
         */
        LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        std::wstring m_className;
        bool m_isHidden;
        std::wstring m_tooltip;
        NotifyIconMenu m_contextMenu;
        HWND m_hwnd;
        GUID m_guid;
        HMENU m_hmenu;
        std::filesystem::path m_openPath;

    private:
        static std::unordered_map<HWND, NotifyIcon*> m_icons;
        /**
         * @brief The window procedure for NotifyIcons
         * @param hwnd HWND
         * @param uMsg UINT
         * @param wParam WPARAM
         * @param lParam LPARAM
         */
        static LRESULT CALLBACK notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

#endif //NOTIFYICON_H
#endif