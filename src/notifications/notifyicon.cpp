#ifdef _WIN32
#include "notifications/notifyicon.h"
#include <stdexcept>
#include <strsafe.h>
#include "app/aura.h"
#include "helpers/codehelpers.h"
#include "helpers/stringhelpers.h"

using namespace Nickvision::App;
using namespace Nickvision::Helpers;

namespace Nickvision::Notifications
{
    std::unordered_map<HWND, NotifyIcon*> NotifyIcon::m_icons = {};

    NotifyIcon::NotifyIcon(HWND hwnd, const std::wstring& tooltip, const NotifyIconMenu& contextMenu, bool hidden)
        : m_className{ StringHelpers::wstr(StringHelpers::newGuid()) },
        m_isHidden{ hidden },
        m_tooltip{ tooltip },
        m_contextMenu{ contextMenu },
        m_hwnd{ nullptr },
        m_guid{ 0 },
        m_hmenu{ nullptr }
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        //Get app icons
        HICON icon{ (HICON)GetClassLongPtrW(hwnd, GCLP_HICON) };
        if (!icon)
        {
            SHSTOCKICONINFO info{ 0 };
            info.cbSize = sizeof(info);
            if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON, &info) == S_OK)
            {
                icon = info.hIcon;
            }
        }
        HICON smallIcon{ (HICON)GetClassLongPtrW(hwnd, GCLP_HICONSM) };
        if (!smallIcon)
        {
            SHSTOCKICONINFO info{ 0 };
            info.cbSize = sizeof(info);
            if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON | SHGSI_SMALLICON, &info) == S_OK)
            {
                icon = info.hIcon;
            }
        }
        //Create a window for the NotifyIcon
        WNDCLASSEXW windowClass;
        windowClass.cbSize = sizeof(windowClass);
        windowClass.style = CS_GLOBALCLASS;
        windowClass.lpfnWndProc = NotifyIcon::notifyIconWindowProc;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = nullptr;
        windowClass.hIcon = icon;
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);;
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        windowClass.lpszMenuName = 0;
        windowClass.lpszClassName = m_className.c_str();
        windowClass.hIconSm = smallIcon;
        ATOM registeredClass{ RegisterClassExW(&windowClass) };
        if (registeredClass == 0)
        {
            throw std::runtime_error("Unable to register window class: " + CodeHelpers::getLastSystemError());
        }
        m_hwnd = CreateWindowExW(0, m_className.c_str(), L"libnickNotifyIcon", WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, nullptr, nullptr, nullptr);
        if (!m_hwnd)
        {
            throw std::runtime_error("Unable to create window: " + CodeHelpers::getLastSystemError());
        }
        m_icons.insert({ m_hwnd, this });
        //Create the NotifyIcon
        if (CoCreateGuid(&m_guid) != S_OK)
        {
            throw std::runtime_error("Unable to create guid: " + CodeHelpers::getLastSystemError());
        }
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_ICON | NIF_TIP;
        notify.hIcon = icon;
        StringCchCopyW(notify.szTip, ARRAYSIZE(notify.szTip), m_tooltip.c_str());
        notify.uVersion = NOTIFYICON_VERSION_4;
        if (!Shell_NotifyIconW(NIM_ADD, &notify))
        {
            throw std::runtime_error("Unable to create NotifyIcon: " + CodeHelpers::getLastSystemError());
        }
        Shell_NotifyIconW(NIM_SETVERSION, &notify);
        //Create context menu
        if(!createContextMenu())
        {
            throw std::runtime_error("Unable to create context menu: " + CodeHelpers::getLastSystemError());
        }
        //Use dark mode if needed
        HMODULE uxTheme{ LoadLibraryExA("uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32) };
        bool(WINAPI* ShouldAppsUseDarkMode)(){ (bool(WINAPI*)())GetProcAddress(uxTheme, MAKEINTRESOURCEA(132)) };
        int(WINAPI* SetPreferredAppMode)(int){ (int(WINAPI*)(int))GetProcAddress(uxTheme, MAKEINTRESOURCEA(135)) };
        if(ShouldAppsUseDarkMode())
        {
            SetPreferredAppMode(2); //2 == ForceDark
        }
        FreeLibrary(uxTheme);
    }

    NotifyIcon::~NotifyIcon()
    {
        //Destroy NotifyIcon
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        Shell_NotifyIconW(NIM_DELETE, &notify);
        //Destroy context menu
        if (m_hmenu)
        {
            DestroyMenu(m_hmenu);
        }
        //Destroy window
        m_icons.erase(m_hwnd);
        DestroyWindow(m_hwnd);
        UnregisterClassW(m_className.c_str(), nullptr);
    }

    bool NotifyIcon::hide()
    {
        m_isHidden = true;
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        return Shell_NotifyIconW(NIM_MODIFY, &notify) == TRUE;
    }

    bool NotifyIcon::show()
    {
        m_isHidden = false;
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        return Shell_NotifyIconW(NIM_MODIFY, &notify) == TRUE;
    }

    const std::wstring& NotifyIcon::getTooltip() const
    {
        return m_tooltip;
    }

    bool NotifyIcon::setTooltip(const std::wstring& tooltip)
    {
        m_tooltip = tooltip;
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_TIP;
        StringCchCopyW(notify.szTip, ARRAYSIZE(notify.szTip), m_tooltip.c_str());
        return Shell_NotifyIconW(NIM_MODIFY, &notify) == TRUE;
    }

    const NotifyIconMenu& NotifyIcon::getContextMenu() const
    {
        return m_contextMenu;
    }

    bool NotifyIcon::setContextMenu(const NotifyIconMenu& menu)
    {
        m_contextMenu = menu;
        return createContextMenu();
    }

    bool NotifyIcon::notify(const ShellNotificationSentEventArgs& e)
    {
        std::wstring message{ StringHelpers::wstr(e.getMessage()) };
        std::wstring title{ StringHelpers::wstr(e.getTitle()) };
        NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_INFO;
        StringCchCopyW(notify.szInfo, ARRAYSIZE(notify.szInfo), message.c_str());
        StringCchCopyW(notify.szInfoTitle, ARRAYSIZE(notify.szInfoTitle), title.c_str());
        notify.dwInfoFlags = NIIF_RESPECT_QUIET_TIME;
        if (e.getSeverity() == NotificationSeverity::Warning)
        {
            notify.dwInfoFlags |= NIIF_WARNING;
        }
        else if(e.getSeverity() == NotificationSeverity::Error)
        {
            notify.dwInfoFlags |= NIIF_ERROR;
        }
        else
        {
            notify.dwInfoFlags |= NIIF_NONE;
        }
        if (e.getAction() == "open" && std::filesystem::exists(e.getActionParam()))
        {
            m_openPath = e.getActionParam();
        }
        else
        {
            m_openPath = std::filesystem::path();
        }
        return Shell_NotifyIconW(NIM_MODIFY, &notify) == TRUE;
    }

    NOTIFYICONDATAW NotifyIcon::getBaseNotifyIconData()
    {
        NOTIFYICONDATAW notify{ 0 };
        notify.cbSize = sizeof(notify);
        notify.hWnd = m_hwnd;
        notify.guidItem = m_guid;
        notify.uFlags = NIF_MESSAGE | NIF_GUID | NIF_SHOWTIP;
        if (m_isHidden)
        {
            notify.uFlags |= NIF_STATE;
            notify.dwState = NIS_HIDDEN;
        }
        else
        {
            notify.uFlags |= NIF_STATE;
            notify.dwState = 0;
        }
        notify.uCallbackMessage = WM_NOTIFYICON_EVENT;
        return notify;
    }

    bool NotifyIcon::createContextMenu()
    {
        if(m_hmenu)
        {
            DestroyMenu(m_hmenu);
            m_hmenu = nullptr;
        }
        if(!m_contextMenu.empty())
        {
            m_hmenu = CreatePopupMenu();
            if (!m_hmenu)
            {
                std::cout << "here1" << std::endl;
                return false;
            }
            for (size_t i = 0; i < m_contextMenu.size(); i++)
            {
                const std::shared_ptr<NotifyIconMenuItem>& item{ m_contextMenu[i] };
                if (item->getType() == NotifyIconMenuItemType::Action)
                {
                    const NotifyIconActionMenuItem* action{ static_cast<NotifyIconActionMenuItem*>(item.get()) };
                    std::wstring label{ StringHelpers::wstr(action->getLabel()) };
                    MENUITEMINFOW actionItem{ 0 };
                    actionItem.cbSize = sizeof(actionItem);
                    actionItem.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STRING;
                    actionItem.fType = MFT_STRING;
                    actionItem.wID = static_cast<unsigned int>(IDM_CONTEXT_MENU + i);
                    actionItem.dwTypeData = LPWSTR(label.c_str());
                    actionItem.cch = static_cast<unsigned int>(label.size());
                    InsertMenuItemW(m_hmenu, static_cast<unsigned int>(i), TRUE, &actionItem);
                }
                else if (item->getType() == NotifyIconMenuItemType::Separator)
                {
                    MENUITEMINFOW separatorItem{ 0 };
                    separatorItem.cbSize = sizeof(separatorItem);
                    separatorItem.fMask = MIIM_ID | MIIM_FTYPE;
                    separatorItem.fType = MFT_SEPARATOR;
                    separatorItem.wID = static_cast<unsigned int>(IDM_CONTEXT_MENU + i);
                    InsertMenuItemW(m_hmenu, static_cast<unsigned int>(i), TRUE, &separatorItem);
                }
            }
        }
        return true;
    }

    LRESULT NotifyIcon::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_NOTIFYICON_EVENT) //NotifyIcon event
        {
            if (LOWORD(lParam) == NIN_SELECT || LOWORD(lParam) == WM_CONTEXTMENU) //Icon clicked or right-clicked
            {
                //Attach context menu to NotifyIcon window
                POINT point{ LOWORD(wParam), HIWORD(wParam) };
                SetForegroundWindow(m_hwnd);
                UINT flags{ TPM_RIGHTBUTTON };
                if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
                {
                    flags |= TPM_RIGHTALIGN;
                }
                else
                {
                    flags |= TPM_LEFTALIGN;
                }
                TrackPopupMenuEx(m_hmenu, flags, point.x, point.y, m_hwnd, nullptr);
                return 0;
            }
            else if (LOWORD(lParam) == NIN_BALLOONUSERCLICK) //Ballon clicked ("open" event)
            {
                if (std::filesystem::exists(m_openPath))
                {
                    ShellExecuteW(nullptr, L"open", m_openPath.wstring().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
                }
            }
            NOTIFYICONDATAW notify{ getBaseNotifyIconData() };
            Shell_NotifyIconW(NIM_MODIFY, &notify);
            return 0;
        }
        else if (uMsg == WM_COMMAND) //ContextMenu event
        {
            int index{ LOWORD(wParam) - IDM_CONTEXT_MENU };
            const std::shared_ptr<NotifyIconMenuItem> item{ m_contextMenu[index] };
            if(item)
            {
                if (item->getType() == NotifyIconMenuItemType::Action)
                {
                    const NotifyIconActionMenuItem* action{ static_cast<NotifyIconActionMenuItem*>(item.get()) };
                    action->invoke();
                }
                return 0;
            }
        }
        return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);
    }

    LRESULT CALLBACK NotifyIcon::notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (m_icons.contains(hwnd))
        {
            return m_icons[hwnd]->handleMessage(uMsg, wParam, lParam);
        }
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}
#endif //_WIN32