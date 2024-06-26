# Nickvision::Taskbar

This module contains objects for manipulating an application's taskbar button.

## Table of Contents
- [ProgressState](#progressstate)
- [TaskbarItem](#taskbaritem)

## ProgressState
Description: States of progress on a taskbar button.

Interface: [progressstate.h](/include/taskbar/progressstate.h)

Type: `enum class`

Path: `Nickvision::Taskbar::ProgressState`

### Values
- ```
  NoProgress = 0
  ```
    - Represents no progress being shown on the taskbar.
- ```
  Indeterminate = 1
  ```
    - Represents indeterminate progress (no specific value) being shown on the taskbar.
- ```
  Normal = 2
  ```
    - Represents normal progress being shown on the taskbar.
- ```
  Error = 4
  ```
    - Represents error progress being shown on the taskbar.
- ```
  Paused = 8
  ```
    - Represents paused progress being shown on the taskbar.

## TaskbarItem
Description: An item on the taskbar.

Interface: [taskbaritem.h](/include/taskbar/taskbaritem.h)

Type: `class`

Path: `Nickvision::Taskbar::TaskbarItem`

### Member Variables
- ```
  Nickvision::Taskbar::ProgressState ProgressState: get, set
  ```
    - The state of the progress being shown.
- ```
  double Progress: get, set
  ```
    - The value of the progress. Should be a value between 0 and 1.
    - Note: Setting the progress value will set the progress state to Normal if progress > 0, else will set progress state to NoProgress.
- ```
  bool Urgent: get, set
  ```
    - Whether or not the taskbar item is shown in an urgent state.
- ```
  bool CountVisible: get, set
  ```
    - Whether or not the count is visible on the taskbar item.
- ```
  long Count: get, set
  ```
    - The count shown on the taskbar item.

### Methods
- ```cpp
  TaskbarItem()
  ```
    - Constructs a TaskbarItem.
- ```cpp
  ~TaskbarItem()
  ``` 
    - Destructs a TaskbarItem.
- ```cpp
  bool connect(HWND hwnd)
  ``` 
    - Accepts: The HWND handle of the main application window, hwnd.
    - Returns: `true` if connected to the taskbar item.
    - Returns: `false` if unable to connect to the taskbar item.
    - Note: This method is only available on Windows.
- ```cpp
  bool connect(const std::string& desktopFile)
  ``` 
    - Accepts: The desktop file name with the file extension of the running application, desktopFile.
    - Returns: `true` if connected to the taskbar item.
    - Returns: `false` if unable to connect to the taskbar item.
    - Note: This method is only available on Linux.