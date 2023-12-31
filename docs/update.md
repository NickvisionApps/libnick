# Nickvision::Aura::Update

This module contains objects for fetching and updating Nickvision applications.

## Table of Contents
- [Updater](#updater)

## Updater
Description: Event args for when a notification is sent.

Interface: [updater.h](/include/update/updater.h)

Type: `class`

Path: `Nickvision::Aura::Update::Updater`

### Methods
- ```cpp
  Updater(std::string githubRepoUrl = "")
  ```  
     - Constructs an Updater.
     - Accepts: The url of a GitHub repo to check for updates, githubRepoUrl. If githubRepoUrl is empty, the url will attempt to be determined from `Aura::getActive().getAppInfo().getSourceRepo()`.
     - Throws: `std::invalid_argument` if no valid GitHub url can be determined.
- ```cpp
  Nickvision::Aura::Version fetchCurrentStableVersion()
  ``` 
    - Returns: The latest stable Version found in the repo.
    - Returns: An empty versino on error.
    - Note: This method scans the tags of the repo and parses versions in the format `major.minor.build`.
- ```cpp
  Nickvision::Aura::Version fetchCurrentPreviewVersion()
  ``` 
    - Returns: The latest preview (dev) Version found in the repo.
    - Returns: An empty versino on error.
    - Note: This method scans the tags of the repo and parses versions in the format `major.minor.build-dev`.
- ```cpp
  bool windowsUpdate(VersionType versionType)
  ``` 
    - Accepts: The type of version to download and install, versionType.
    - Returns: `true` if the update was downloaded and launched successfully.
    - Returns: `false` if an error occured when attempting to update the app.
    - Note: Windows application installers should end in `setup.exe` to be fetched by the Updater.
    - Note: This method if only available on the Windows platform.