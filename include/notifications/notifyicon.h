/**
 * @file
 * @author Nicholas Logozzo <nlogozzo225@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * An icon for the Windows system tray.
 */

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

namespace Nickvision::Notifications
{
    /**
     * @brief An icon for the Windows system tray.
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
