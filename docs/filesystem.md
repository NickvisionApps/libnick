# Nickvision::Filesystem

This module contains objects for working with the filesystem, such as a file system watcher.

## Table of Contents
- [FileAction](#fileaction)
- [FileSystemChangedEventArgs](#filesystemchangedeventargs)
- [FileSystemWatcher](#filesystemwatcher)
- [UserDirectories](#userdirectories)
- [UserDirectory](#userdirectory)
- [WatcherFlags](#watcherflags)

## FileAction
Description: Actions that cause a file system object to change. 

Interface: [fileaction.h](/include/filesystem/fileaction.h)

Type: `enum class`

Path: `Nickvision::Filesystem::FileAction`

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

## FileSystemChangedEventArgs
Description: Event args for when a file system object is changed.

Interface: [filesystemchangedeventargs.h](/include/filesystem/filesystemchangedeventargs.h)

Type: `class`

Path: `Nickvision::Filesystem::FileSystemChangedEventArgs`

### Member Variables
- ```cpp
  std::filesystem::path Path: get
  ```
    - The path of the changed file system object system object. 
- ```cpp
  Nickvision::Filesystem::FileAction Why: get
  ```
    - The action that caused the file system object to change.

## FileSystemWatcher
Description: A watcher of a file system folder.

Interface: [filesystemwatcher.h](/include/filesystem/filesystemwatcher.h)

Type: `class`

Path: `Nickvision::Filesystem::FileSystemWatcher`

### Member Variables
- ```
  std::filesystem::path Path: get
  ``` 
    - The path of the folder being watched.
- ```
  Nickvision:Aura::Filesystem::WatcherFlags WatcherFlags: get
  ``` 
    - The flags of what to watch changes for.
- ```
  bool IncludeSubdirectories: get
  ``` 
    - Whether or not subdirectories of the folder are watched.

### Events
- ```cpp
  Event<Nickvision::Filesystem::FileSystemChangedEventArgs> Changed
  ``` 
    - Invoked when a watched flag of the folder is changed.

### Methods
- ```cpp
  FileSystemWatcher(const std::filesystem::path& path, bool includeSubdirectories, WatcherFlags watcherFlags = WatcherFlags::FileName | WatcherFlags::DirectoryName | WatcherFlags::Attributes | WatcherFlags::Size | WatcherFlags::LastWrite | WatcherFlags::LastAccess)
  ```
    - Constructs a FileSystemWatcher.
    - Accepts: The path of the folder to watch, path, whether or not to watch subdirectories of said folder, includeSubdirectories, and the set of watcher flags, watcherFlags.
- ```cpp
  ~FileSystemWatcher()
  ``` 
    - Destructs a FileSystemWatcher.
- ```cpp
  bool isExtensionWatched(const std::filesystem::path& extension)
  ```
    - Accepts: The extension to check for if it is being watched, extension.
    - Returns: `true` if the extension is being watched.
    - Returns: `false` if the extension is not being watched.
    - Returns: `true` if the list of extension filters is empty (implying all extensions are being implicityly watched).
- ```cpp
  bool addExtensionFilter(const std::filesystem::path& extension)
  ```
    - Accepts: The extension to add to the list of watched filters, extension.
    - Returns: `true` if the extension was added to the filters list.
    - Returns: `false` if the extension was not added to the filters list. This could mean that the extension was already in the filters list.
- ```cpp
  bool removeExtensionFilter(const std::filesystem::path& extension)
  ```
    - Accepts: The extension to remove from the list of watched filters, extension.
    - Returns: `true` if the extension was removed from the filters list.
    - Returns: `false` if the extension was not removed from the filters list. This could mean that the extension was not in the filters list to begin with.
- ```cpp
  bool clearExtensionFilters()
  ``` 
    - Returns: `true` if the list of extension filters was cleared.
    - Returns: `false` if the list of extension filters was not cleared.

### Extension Filters
The `FileSystemWatcher` object allows the consumer to specify specific extensions of files to watch changes for.

By default (and when the extension filters list is empty), all files, regardless of extension, are watched for changes. 

Otherwise, only the extensions contained in the extension filters list will be watched for changes.

## UserDirectories
Description: Functions for working with user directories.

Interface: [userdirectories.h](/include/filesystem/userdirectories.h)

Type: `namespace`

Path: `Nickvision::Filesystem::UserDirectories`

### Functions
- ```cpp
  std::filesystem::path get(UserDirectory directory, const std::string& appName)
  ```
    - Accepts: A supported user directory to get, directory, and an application name for application specific directories, appName.
    - Returns: The path to the user directory.

## WatcherFlags
Description: Flags to describe properties of a file system object that can change.  

Interface: [watcherflags.h](/include/filesystem/watcherflags.h)

Type: `enum class`

Path: `Nickvision::Filesystem::WatcherFlags`

### Values
- ```cpp
  FileName = 1
  ```
    - Signifies to watch for file name changes.
- ```cpp
  DirectoryName = 2
  ``` 
    - Signifies to watch for directory name changes.
- ```cpp
  Attributes = 4
  ``` 
    - Signifies to watch for file attribute changes.
- ```cpp
  Size = 8
  ``` 
    - Signifies to watch for file/directory size changes.
- ```cpp
  LastWrite = 16
  ``` 
    - Signifies to watch for last file write time changes.
- ```cpp
  LastAccess = 32
  ``` 
    - Signifies to watch for last file access time changes.

## UserDirectory
Description: Supported user directories.

Interface: [userdirectory.h](/include/filesystem/userdirectory.h)

Type: `enum class`

Path: `Nickvision::Filesystem::UserDirectory`

### Values
- ```cpp
  Home = 0
  ```
- ```cpp
  Config = 1
  ```
- ```cpp
  ApplicationConfig = 2
  ```
- ```cpp
  Cache = 3
  ```
- ```cpp
  ApplicationCache = 4
  ```
- ```cpp
  LocalData = 5
  ```
- ```cpp
  ApplicationLocalData = 6
  ```
- ```cpp
  Desktop = 7
  ```
- ```cpp
  Documents = 8
  ```
- ```cpp
  Downloads = 9
  ```
- ```cpp
  Music = 10
  ```
- ```cpp
  Pictures = 11
  ```
- ```cpp
  Templates = 12
  ```
- ```cpp
  Videos = 13
  ```