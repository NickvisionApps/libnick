#ifdef _WIN32
#include "notifications/notifyicon.h"
#include <stdexcept>
#include <strsafe.h>
#include "app/aura.h"
#include "helpers/stringhelpers.h"

using namespace Nickvision::App;

namespace Nickvision::Notifications
{
    std::map<HWND, NotifyIcon*> NotifyIcon::m_icons = {};

    NotifyIcon::NotifyIcon(HWND hwnd, const NotifyIconMenu& contextMenu, bool hidden)
        : m_className{ StringHelpers::newGuid() },
        m_isHidden{ hidden },
        m_tooltip{ Aura::getActive().getAppInfo().getName() },
        m_contextMenu{ contextMenu },
        m_hwnd{ nullptr }
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        HICON icon{ (HICON)GetClassLongPtrA(hwnd, GCLP_HICON) };
        if (!icon)
        {
            SHSTOCKICONINFO info{ 0 };
            info.cbSize = sizeof(info);
            if (SHGetStockIconInfo(SIID_APPLICATION, SHGSI_ICON, &info) == S_OK)
            {
                icon = info.hIcon;
            }
        }
        HICON smallIcon{ (HICON)GetClassLongPtrA(hwnd, GCLP_HICONSM) };
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
        WNDCLASSEXA windowClass;
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
        ATOM registeredClass{ RegisterClassExA(&windowClass) };
        if (registeredClass == 0)
        {
            throw std::runtime_error("Unable to register window class: " + std::to_string(GetLastError()) + ".");
        }
        m_hwnd = CreateWindowExA(0, m_className.c_str(), "AuraNotifyIcon", WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, nullptr, nullptr, nullptr);
        if (!m_hwnd)
        {
            throw std::runtime_error("Unable to create window: " + std::to_string(GetLastError()) + ".");
        }
        m_icons.insert({ m_hwnd, this });
        //Create the NotifyIcon
        if (CoCreateGuid(&m_guid) != S_OK)
        {
            throw std::runtime_error("Unable to create guid: " + std::to_string(GetLastError()) + ".");
        }
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_ICON | NIF_TIP;
        notify.hIcon = icon;
        StringCchCopyA(notify.szTip, ARRAYSIZE(notify.szTip), m_tooltip.c_str());
        notify.uVersion = NOTIFYICON_VERSION_4;
        if (!Shell_NotifyIconA(NIM_ADD, &notify))
        {
            throw std::runtime_error("Unable to create NotifyIcon: " + std::to_string(GetLastError()) + ".");
        }
        Shell_NotifyIconA(NIM_SETVERSION, &notify);
        //Create context menu
        if (!m_contextMenu.empty())
        {
            m_hmenu = CreatePopupMenu();
            if (!m_hmenu)
            {
                throw std::runtime_error("Unable to create menu: " + std::to_string(GetLastError()) + ".");
            }
            for (size_t i = 0; i < m_contextMenu.size(); i++)
            {
                const std::shared_ptr<NotifyIconMenuItem>& item{ m_contextMenu.get(i) };
                if (item->getType() == NotifyIconMenuItemType::Action)
                {
                    const NotifyIconActionMenuItem* action{ static_cast<NotifyIconActionMenuItem*>(item.get()) };
                    MENUITEMINFOA actionItem{ 0 };
                    actionItem.cbSize = sizeof(actionItem);
                    actionItem.fMask = MIIM_ID | MIIM_FTYPE | MIIM_STRING;
                    actionItem.fType = MFT_STRING;
                    actionItem.wID = static_cast<unsigned int>(IDM_CONTEXT_MENU + i);
                    actionItem.dwTypeData = LPSTR(action->getLabel().c_str());
                    actionItem.cch = static_cast<unsigned int>(action->getLabel().size());
                    InsertMenuItemA(m_hmenu, static_cast<unsigned int>(i), TRUE, &actionItem);
                }
                else if (item->getType() == NotifyIconMenuItemType::Separator)
                {
                    MENUITEMINFOA separatorItem{ 0 };
                    separatorItem.cbSize = sizeof(separatorItem);
                    separatorItem.fMask = MIIM_ID | MIIM_FTYPE;
                    separatorItem.fType = MFT_SEPARATOR;
                    separatorItem.wID = static_cast<unsigned int>(IDM_CONTEXT_MENU + i);
                    InsertMenuItemA(m_hmenu, static_cast<unsigned int>(i), TRUE, &separatorItem);
                }
            }
        }
    }

    NotifyIcon::~NotifyIcon()
    {
        //Destroy NotifyIcon
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        Shell_NotifyIconA(NIM_DELETE, &notify);
        //Destroy window for NotifyIcon
        if (m_hmenu)
        {
            DestroyMenu(m_hmenu);
        }
        m_icons.erase(m_hwnd);
        DestroyWindow(m_hwnd);
        UnregisterClassA(m_className.c_str(), nullptr);
    }

    bool NotifyIcon::hide()
    {
        m_isHidden = true;
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        return Shell_NotifyIconA(NIM_MODIFY, &notify) == TRUE;
    }

    bool NotifyIcon::show()
    {
        m_isHidden = false;
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        return Shell_NotifyIconA(NIM_MODIFY, &notify) == TRUE;
    }

    const std::string& NotifyIcon::getTooltip() const
    {
        return m_tooltip;
    }

    bool NotifyIcon::setTooltip(const std::string& tooltip)
    {
        m_tooltip = tooltip;
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_TIP;
        StringCchCopyA(notify.szTip, ARRAYSIZE(notify.szTip), m_tooltip.c_str());
        return Shell_NotifyIconA(NIM_MODIFY, &notify) == TRUE;
    }

    bool NotifyIcon::notify(const ShellNotificationSentEventArgs& e)
    {
        NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
        notify.uFlags |= NIF_INFO;
        StringCchCopyA(notify.szInfo, ARRAYSIZE(notify.szInfo), e.getMessage().c_str());
        StringCchCopyA(notify.szInfoTitle, ARRAYSIZE(notify.szInfoTitle), e.getTitle().c_str());
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
        return Shell_NotifyIconA(NIM_MODIFY, &notify) == TRUE;
    }

    NOTIFYICONDATAA NotifyIcon::getBaseNotifyIconData()
    {
        NOTIFYICONDATAA notify{ 0 };
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

    LRESULT NotifyIcon::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_NOTIFYICON_EVENT)
        {
            if (LOWORD(lParam) == NIN_SELECT || LOWORD(lParam) == WM_CONTEXTMENU)
            {
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
            else if (LOWORD(lParam) == NIN_BALLOONUSERCLICK)
            {
                if (!m_openPath.empty() && std::filesystem::exists(m_openPath))
                {
                    ShellExecuteA(nullptr, "open", m_openPath.string().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
                }
            }
            NOTIFYICONDATAA notify{ getBaseNotifyIconData() };
            Shell_NotifyIconA(NIM_MODIFY, &notify);
            return 0;
        }
        else if (uMsg == WM_COMMAND)
        {
            int index{ LOWORD(wParam) - IDM_CONTEXT_MENU };
            if (m_contextMenu.size() != 0 && index >= 0 && index < m_contextMenu.size())
            {
                const std::shared_ptr<NotifyIconMenuItem> item{ m_contextMenu.get(index) };
                if (item->getType() == NotifyIconMenuItemType::Action)
                {
                    const NotifyIconActionMenuItem* action{ static_cast<NotifyIconActionMenuItem*>(item.get()) };
                    action->invoke();
                }
                return 0;
            }
        }
        return DefWindowProcA(m_hwnd, uMsg, wParam, lParam);
    }

    LRESULT NotifyIcon::notifyIconWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (m_icons.contains(hwnd))
        {
            return m_icons[hwnd]->handleMessage(uMsg, wParam, lParam);
        }
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
}
#endif