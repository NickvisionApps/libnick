# Nickvision::Aura::Filesystem

This module contains objects for working with the filesystem, such as a file system watcher.

## Table of Contents
- [FileAction](#fileaction)
- [FileSystemChangedEventArgs](#filesystemchangedeventargs)
- [FileSystemWatcher](#filesystemwatcher)
- [WatcherFlags](#watcherflags)


## FileAction
Description: Actions that cause a file system object to change. 

Interface: [fileaction.h](/include/filesystem/fileaction.h)

Type: `enum class`

Path: `Nickvision::Aura::Filesystem::FileAction`

### Values
- ```cpp
  Added = 1
  ```
    - Represents a file being added
- ```cpp
  Removed = 2
  ```
    - Represents a file being removed
- ```cpp
  Modified = 3
  ```
    - Represents a file being modified
- ```cpp
  Renamed = 4
  ```
    - Represents a file being renamed