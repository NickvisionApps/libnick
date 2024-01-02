# Nickvision::Aura::Notifications

This module contains objects for sending notifications within Nickvision applications.

## Table of Contents
- [NotificationSentEventArgs](#notificationsenteventargs)
- [NotificationSeverity](#notificationseverity)
- [NotifyIcon](#notifyicon)
- [NotifyIconMenu](#notifyiconmenu)
- [ShellNotification](#shellnotification)
- [ShellNotificationSentEventArgs](#shellnotificationsenteventargs)

## NotificationSentEventArgs
Description: Event args for when a notification is sent.

Interface: [notificationsenteventargs.h](/include/notifications/notificationsenteventargs.h)

Type: `class`

Path: `Nickvision::Aura::Notifications::NotificationSentEventArgs`

### Member Variables
- ```
  std::string Message: get
  ```
    - The message of the notification.
- ```
  Nickvision::Aura::Notifications::NotificationSeverity Severity: get
  ```
    - The severity of the notification.
- ```
  std::string Action: get
  ```
    - The optional action of the notification.
- ```
  std::string ActionParam: get
  ```
    - The parameter of the optional action.

### Methods
- ```cpp
  NotificationSentEventArgs(const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "")
  ``` 
    - Constructs a NotificationSentEventArgs.
    - Accepts: The notification message, message, the severity of the notification, severity, an optional action for the notification, action, and an optional action parameter for the action, actionParam.

## NotificationSeverity
Description: Severities for a notification.

Interface: [notificationseverity.h](/include/notifications/notificationseverity.h)

Type: `enum class`

Path: `Nickvision::Aura::Notifications::NotificationSeverity`

### Values
- ```
  Informational = 0
  ``` 
    - Represents an informational notification
- ```
  Success = 1
  ``` 
    - Represents a successful notification
- ```
  Warning = 2
  ``` 
    - Represents a warning notification
- ```
  Error = 3
  ``` 
    - Represents an error notification

## NotifyIcon
Description: An icon for the system tray.

Note: This API is only available on the Windows platform.

Interface: [notifyicon.h](/include/notifications/notifyicon.h)

Type: `class`

Path: `Nickvision::Aura::Notifications::NotifyIcon`

### Methods
- ```cpp
  NotifyIcon(HWND hwnd, const NotifyIconMenu& contextMenu = { }, bool hidden = false);
  ``` 
    - Constructs a NotifyIcon.
    - Accepts: The `HWND` handle for the main application window, hwnd, the context menu model for the icon, contextMenu, and whether or not to hide the icon by default, hidden.
    - Throws: std::logic_error if Aura::init() was not called yet.
    - Throws: std::runtime_error if unable to create the NotifyIcon.
- ```cpp
  ~NotifyIcon()
  ``` 
    - Destructs a NotifyIcon.
- ```cpp
  bool hide()
  ``` 
    - Returns: `true` if the icon was hidden.
    - Returns: `false` if the icon was not hidden.
- ```cpp
  bool show()
  ``` 
    - Returns: `true` if the icon was shown.
    - Returns: `false` if the icon was not shown.
- ```cpp
  const std::string& getTooltip() const
  ```
    - Returns: The tooltip text of the icon.
- ```cpp
  bool setTooltip(const std::string& tooltip)
  ```
    - Accepts: The tooltip text to show on the icon, tooltip.
    - Returns: `true` if the tooltip was updated.
    - Returns: `false` if the tooltip was not updated.
- ```cpp
  bool notify(const ShellNotificationSentEventArgs& e)
  ``` 
    - Accepts: The `ShellNotificationSentEventArgs` containing the information to show on the NotifyIcon, e.
    - Returns: `true` if the notification was shown from the NotifyIcon.
    - Returns: `false` if the notification was not shown from the NotifyIcon.
    - Note: Supports the action "open" with action param being a path of a file or folder to open.

## NotifyIconMenu
Description: A menu for a NotifyIcon.

Note: This API is only available on the Windows platform.

Interface: [notifyiconmenu.h](/include/notifications/notifyiconmenu.h)

Type: `class`

Path: `Nickvision::Aura::Notifications::NotifyIconMenu`

Note: There are other classes, notably: `NotifyIconMenuItem`, `NotifyIconSeparatorMenuItem`, `NotifyIconActionMenuItem`, that are meant for use by `NotifyIcon`'s implementation of the context menu itself. Users are free to use these objects, however they will not be covered in this documentation.

### Methods
- ```cpp
  size_t size() const
  ```
    - Returns: The number of items stored in the menu.
- ```cpp
  bool empty() const
  ```
    - Returns: `true` if the menu is empty.
    - Returns: `false` if the menu is not empty.
- ```cpp
  size_t addSeparator()
  ```
    - Returns: The index at which the new separator was added.
- ```cpp
  bool insertSeparator(size_t index)
  ```
    - Accepts: The index at which to insert a new separator, index.
    - Returns: `true` if the separator was inserted.
    - Returns: `false` if the separator was not inserted.
- ```cpp
  bool removeSeparator(size_t index)
  ```
    - Accepts: The index at which to remove a separator, index.
    - Returns: `true` if the separator was removed.
    - Returns: `false` if the separator was not removed. This could mean that the index was invalid or that the index pointed a menu item that was not a separator.
- ```cpp
  size_t addAction(const std::string& label, const std::function<void()>& action)
  ```
    - Accepts: The label of the new action item, label, and the callback function of the action, action.
    - Returns: The index at which the new action item was added.
- ```cpp
  bool insertAction(size_t index, const std::string& label, const std::function<void()>& action)
  ```
    - Accepts: The index at which to insert a new action item, index, the label of the new action, label, and the callback function of the action, action.
    - Returns: `true` if the action item was inserted.
    - Returns: `false` if the action item was not inserted.
- ```cpp
  bool removeAction(size_t index)
  ```
    - Accepts: The index at which to remove an action item, index.
    - Returns: `true` if the action item was removed.
    - Returns: `false` if the action item was not removed. This could mean that the index was invalid or that the index pointed a menu item that was not an action item.

### Creating a Context Menu
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
NotifyIcon icon{ GetConsoleWindow(), contextMenu };
...
```
## ShellNotification
Description: Functions for working with shell (desktop) notifications.

Interface: [shellnotification.h](/include/notifications/shellnotification.h)

Type: `namespace`

Path: `Nickvision::Aura::Notifications::ShellNotification`

### Functions
- ```cpp
  void send(const ShellNotificationSentEventArgs& e, HWND hwnd)
  ``` 
    - Accepts: The `ShellNotificationSentEventArgs` containing the information to show, e, and the `HWND` handle for the main application window, hwnd. The hwnd parameter is only used once on the initial creation of the static `NotifyIcon` and then is ignored on future calls.
    - Note: This function is only available on the Windows platform. It uses `Nickvision::Aura::Notifications::NotifyIcon`.
    - Note: This function supports the action "open" with action param being a path of a file or folder to open.
    - Throws: std::logic_error if Aura::init() was not called yet 
- ```cpp
  void send(const ShellNotificationSentEventArgs& e)
  ``` 
    - Accepts: The `ShellNotificationSentEventArgs` containing the information to show, e.
    - Note: This function is only available on the Linux platform.
    - Note: This function supports the action "open" with action param being a path of a file or folder to open. The app must define an "app.open" action to handle this event.
    - Throws: std::logic_error if Aura::init() was not called yet 

## ShellNotificationSentEventArgs
Description: Event args for when a shell notification is sent.

Interface: [shellnotificationsenteventargs.h](/include/notifications/shellnotificationsenteventargs.h)

Type: `class`

Path: `Nickvision::Aura::Notifications::ShellNotificationSentEventArgs`

### Member Variables
- ```
  std::string Title: get
  ```
    - The title of the notification.
- ```
  std::string Message: get
  ```
    - The message of the notification.
- ```
  Nickvision::Aura::Notifications::NotificationSeverity Severity: get
  ```
    - The severity of the notification.
- ```
  std::string Action: get
  ```
    - The optional action of the notification.
- ```
  std::string ActionParam: get
  ```
    - The parameter of the optional action.

### Methods
- ```cpp
  ShellNotificationSentEventArgs(const std::string& title, const std::string& message, NotificationSeverity severity, const std::string& action = "", const std::string& actionParam = "")
  ``` 
    - Constructs a ShellNotificationSentEventArgs.
    - Accepts: The notification title, title, the notification message, message, the severity of the notification, severity, an optional action for the notification, action, and an optional action parameter for the action, actionParam.