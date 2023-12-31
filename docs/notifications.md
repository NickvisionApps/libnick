# Nickvision::Aura::Notifications

This module contains objects for sending notifications within Nickvision applications.

## Table of Contents
- [NotificationSentEventArgs](#notificationsenteventargs)
- [NotificationSeverity](#notificationseverity)
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
    - Accepts: The notification title, title, the notification message, message, the severity of the notification, severity, an optional action for the notification, action, and an optional action parameter for the action, actionParam.