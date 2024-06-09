# Nickvision::Update

This module contains objects for fetching and updating Nickvision applications.

## Table of Contents
- [Updater](#updater)
- [Version](#version)
- [VersionType](#versiontype)

## Updater
Description: Event args for when a notification is sent.

Interface: [updater.h](/include/update/updater.h)

Type: `class`

Path: `Nickvision::Update::Updater`

### Methods
- ```cpp
  Updater(std::string githubRepoUrl)
  ```  
     - Constructs an Updater.
     - Accepts: The url of a GitHub repo to check for updates, githubRepoUrl.
     - Throws: `std::invalid_argument` if the GitHub url is not of valid format
- ```cpp
  Nickvision::Version fetchCurrentVersion(VersionType type)
  ``` 
    - Accepts: The type of version to fetch, type.
    - Returns: The latest Version of type found in the repo.
    - Returns: An empty version on error.
    - Note: This method scans the tags of the repo and parses versions in the format `major.minor.build`.
- ```cpp
  bool windowsUpdate(VersionType versionType)
  ``` 
    - Accepts: The type of version to download and install, versionType.
    - Returns: `true` if the update was downloaded and launched successfully.
    - Returns: `false` if an error occurred when attempting to update the app.
    - Note: Windows application installers should end in `setup.exe` to be fetched by the Updater.
    - Note: This method if only available on the Windows platform.

## Version
Description: A model for a version number. Formatted in "major.minor.build-dev".

Interface: [version.h](/include/update/version.h)

Type: `class`

Path: `Nickvision::Update::Version`

### Member Variables
- ```
  Nickvision::VersionType VersionType: get
  ```
    - The type of the version

### Methods
- ```cpp
  Version()
  ```
    - Constructs a blank Version.
- ```cpp
  Version(int major, int minor, int build)
  ```
    - Constructs a Version.
    - Accepts: The major version number, major, the minor version number, minor, and the build version number, build.
- ```cpp
  Version(int major, int minor, int build, const std::string& dev)
  ```
    - Constructs a Version.
    - Accepts: The major version number, major, the minor version number, minor, the build version number, build, and the dev version string, dev.
- ```cpp
  Version(const std::string& version)
  ```
    - Constructs a Version.
    - Accepts: The version as a string to parse, version.
    - Note: version must be in the format of `"major.minor.build-dev"`.
- ```cpp
  int getMajor() const
  ```
    - Returns: The major number of the version.
- ```cpp
  int getMinor() const
  ```
    - Returns: The minor number of the version.
- ```cpp
  int getBuild() const
  ```
    - Returns: The build number of the version.
- ```cpp
  const std::string& getDev() const
  ```
    - Returns: The dev string of the version.
- ```cpp
  const std::string& str() const
  ```
    - Returns: The string representation of the Version
- ```cpp
  bool empty() const
  ```
    - Returns: `true` if the Version object is empty
    - Returns: `false` if the Version object is not empty
- ```cpp
  bool operator<(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this < compare.
    - Returns: `false` if this >= compare.
- ```cpp
  bool operator<=(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this <= compare.
    - Returns: `false` if this > compare.
- ```cpp
  bool operator>(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this > compare.
    - Returns: `false` if this <= compare.
- ```cpp
  bool operator>=(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this >= compare.
    - Returns: `false` if this < compare.
- ```cpp
  bool operator==(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this == compare.
    - Returns: `false` if this != compare.
- ```cpp
  bool operator!=(const Version& compare) const
  ```
    - Accepts: The version object to compare to this object, compare.
    - Returns: `true` if this != compare.
    - Returns: `false` if this == compare.
- ```cpp
  friend std::ostream& operator<<(std::ostream& os, const Version& version)
  ```
    - Accepts: The ostream to output the version to, os, and the Version object to output, version.
    - Returns: A reference to the ostream object accepted by the function.

## VersionType
Description: Types of a version

Interface: [versiontype.h](/include/update/versiontype.h)

Type: `enum class`

Path: `Nickvision::Update::VersionType`

### Values
- ```cpp
  Stable = 0
  ```
    - Represents a stable version
- ```cpp
  Preview = 1
  ```
    - Represents a preview (dev) version