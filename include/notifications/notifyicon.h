#ifndef NOTIFYICON_H
#define NOTIFYICON_H

#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#define WM_NOTIFYICON_EVENT (WM_APP + 100)
#define IDM_CONTEXT_MENU (WM_APP + 200)
#endif

#include <filesystem>
#include <unordered_map>
#include "notifyiconmenu.h"
#include "shellnotificationsenteventargs.h"
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#elif defined(__linux__)
#include <libayatana-appindicator3-0.1/libayatana-appindicator/app-indicator.h>
#endif

namespace Nickvision::Notifications
{
    /**
     * @brief An icon for the system tray.
     */
    class NotifyIcon
    {	
    public:
#ifdef _WIN32
        /**
         * @brief Constructs a NotifyIcon.
         * @param hwnd The HWND handle of the main application window
         * @param menu The model for the context menu of the NotifyIcon
         * @param hidden Whether or not the NotifyIcon should be hidden by default
         * @throw std::runtime_error Thrown if unable to create the NotifyIcon
         */
        NotifyIcon(HWND hwnd, const NotifyIconMenu& contextMenu, bool hidden = false);
#elif defined(__linux__)
        /**
         * @brief Constructs a NotifyIcon (AppIndicator icon).
         * @param menu The model for the context menu of the NotifyIcon
         * @param hidden Whether or not the NotifyIcon should be hidden by default
         */
        NotifyIcon(const NotifyIconMenu& contextMenu, bool hidden = false);
#endif
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
        const std::string& getTooltip() const;
        /**
         * @brief Sets the tooltip text of the icon.
         * @param tooltip The toolip text
         * @return True if the tooltip was updated, else false
         */
        bool setTooltip(const std::string& tooltip);
#ifdef _WIN32
        /**
         * @brief Shows a notification from the icon.
         * @brief Supports the action "open" with action param being a path of a file or folder to open.
         * @param e ShellNotificationSentEventArgs
         * @return True if notification was shown from the icon
         */
        bool notify(const ShellNotificationSentEventArgs& e);
#endif

    private:
        bool m_isHidden;
        std::string m_tooltip;
        NotifyIconMenu m_contextMenu;
#ifdef _WIN32
        /**
         * @brief Gets a basic NOTIFYICONDATAA struct for this NotifyIcon.
         * @return NOTIFYICONDATAA
         */
        NOTIFYICONDATAA getBaseNotifyIconData();
        /**
         * @brief Handles a WM_NOTIFYICON_EVENT message.
         * @param wParam WPARAM
         * @param lParam LPARAM
         * @return LRESULT
         */
        LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        std::string m_className;
        HWND m_hwnd;
        GUID m_guid;
        HMENU m_hmenu;
        std::filesystem::path m_openPath;
#elif defined(__linux__)
        GtkBuilder* m_builder;
        AppIndicator* m_appIndicator;
#endif

    private:
#ifdef _WIN32
        static std::unordered_map<HWND, NotifyIcon*> m_icons;
        /**
         * @brief The window procedure for NotifyIcons
         * @param hwnd HWND
         * @param uMsg UINT
         * @param wParam WPARAM
         * @param lParam LPARAM
         */
        static LRESULT notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
    };
}

#endif //NOTIFYICON_H