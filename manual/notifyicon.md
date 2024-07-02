# Creating a Context Menu
The `NotifyMenuItem` provides an easy API for creating context menus for a `NotifyIcon` to use within your app.

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