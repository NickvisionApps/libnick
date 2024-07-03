# Creating a Context Menu
The `Nickvision::Notifications::NotifyMenuItem` class provides an easy API for creating context menus for a `Nickvision::Notifications::NotifyIcon` to use within your Windows app.

Assume we want to create the following context menu for a `NotifyIcon`:
```
---------------
| Show Window |
| ----------- |
|    Exit     |
--------------- 
```

Here is the code we could use to accomplish this:
```cpp
using namespace Nickvision::Notifications;

...
bool m_running{ true };
NotifyIconMenu contextMenu;
contextMenu.addAction("Show Window", [&]()
{
  showWindow();
});
contextMenu.addSeparator();
contextMenu.addAction("Exit", [&m_running]()
{
  m_running = false;
});
NotifyIcon icon{ GetConsoleWindow(), "", contextMenu };
// The following also works:
// NotifyIcon icon{ GetConsoleWindow(), "" };
// icon.setContextMenu(contextMenu);
...
```