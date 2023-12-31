# Nickvision::Aura::Notifications

This module contains objects for sending notifications within Nickvision applications.

## Table of Contents
- [NotificationSentEventArgs](#notificationsenteventargs)
- [NotificationSeverity](#notificationseverity)
- [NotifyIcon](#notifyicon)
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
  NotifyIcon(HWND hwnd)
  ``` 
    - Constructs a NotifyIcon. The NotifyIcon is shown by default.
    - Accepts: The `HWND` handle for the main application window, hwnd.
    - Throws: std::logic_error if Aura::init() was not called yet 
    - Throws: std::runtime_error if unable to create the NotifyIcon
- ```cpp
  ~NotifyIcon()
  ``` 
    - Destructs a NotifyIcon.
- ```cpp
  void hide()
  ``` 
    - Hides the NotifyIcon.
- ```cpp
  void show()
  ``` 
    - Shows the NotifyIcon.
- ```cpp
  void showShellNotification(const ShellNotificationSentEventArgs& e)
  ``` 
    - Accepts: The `ShellNotificationSentEventArgs` containing the information to show on the NotifyIcon, e.
    - Note: Supports the action "open" with action param being a path of a file or folder to open.

## ShellNotification
Description: Functions for working with shell (desktop) notifications.

Interface: [shellnotification.h](/include/notifications/shellnotification.h)

Type: `namespace`

Path: `Nickvision::Aura::Notifications::ShellNotification`

### Functions
- ```cpp
  void show(const ShellNotificationSentEventArgs& e, HWND hwnd)
  ``` 
    - Accepts: The `ShellNotificationSentEventArgs` containing the information to show, e, and the `HWND` handle for the main application window, hwnd. The hwnd parameter is only used once on the initial creation of the static `NotifyIcon` and then is ignored on future calls.
    - Note: This function is only available on the Windows platform. It uses `Nickvision::Aura::Notifications::NotifyIcon`.
    - Note: This function supports the action "open" with action param being a path of a file or folder to open.
    - Throws: std::logic_error if Aura::init() was not called yet 
- ```cpp
  void show(const ShellNotificationSentEventArgs& e)
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