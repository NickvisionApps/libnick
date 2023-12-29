# Nickvision::Aura

This module contains types and functions every Nickvision application utilizes.

## Table of Contents
- [AppInfo](#appinfo)
- [Aura](#aura)
- [ConfigurationBase](#configurationbase)
- [DependencyLocator](#dependencylocator)
- [EnumFlags](#enumflags)
- [InterProcessCommunicator](#interprocesscommunicator)
- [SystemDirectories](#systemdirectories)
- [UserDirectories](#userdirectories)
- [Version](#version)
- [VersionType](#versiontype)

## AppInfo
Description: A model for the information about an application.

Interface: [appinfo.h](/include/appinfo.h)

Type: `class`

Path: `Nickvision::Aura::AppInfo`

### Member Variables
- ```
  std::string Id: get, set
  ```
    - The application id.
    - Ex: `"org.nickvision.application"`
- ```
  std::string Name: get, set
  ```
    - The application name.
    - Ex: `"Nickvision Application"`
- ```
  std::string ShortName: get, set
  ```
    - The application short name.
    - Ex: `"Application"` (can be translated)
- ```
  std::string EnglishShortName: get, set
  ```
    - The application english short name.
    - Ex: `"Application"` (untranslated)
- ```
  std::string Description: get, set
  ```
    - The application description.
    - Ex: `"A template for Nickvision applications."`
- ```
  Nickvision::Aura::Version Version: get, set
  ```
    - The application version.
    - Ex: `2023.12.0`
- ```
  std::string Changelog: get, set
  ```
    - The application changelog.
    - Ex: `"* Initial Release"`
- ```
  std::string HtmlChangelog: get
  ```
    - The application changelog in html form.
    - Ex: `"<ul><li>Initial Release</li></ul>"`
- ```
  std::string SourceRepo: get, set
  ```
    - The application source repo url.
    - Ex: `"https://github.com/nickvisionapps/application"`
- ```
  std::string IssueTracker: get, set
  ```
    - The application issue tracker url.
    - Ex: `"https://github.com/nickvisionapps/application/issues"`
- ```
  std::string SupportUrl: get, set
  ```
    - The application support url.
    - Ex: `"https://github.com/nickvisionapps/application/discussions"`
- ```
  std::string HtmlDocsStore: get, set
  ```
    - The application html docs store url.
    - Ex: `"https://raw.githubusercontent.com/NickvisionApps/Tagger/main/NickvisionTagger.Shared/Docs/html/"`
- ```
  std::map<std::string, std::string>& ExtraLinks: get
  ```
    - Extra links for the application.
    - Key: Name of the link.
    - Value: URL of the link.
- ```
  std::map<std::string, std::string>& Developers: get
  ```
    - The application developers.
    - Key: Name of the developer.
    - Value: URL or email of the developer.
- ```
  std::map<std::string, std::string>& Designers: get
  ```
    - The application designers.
    - Key: Name of the designer.
    - Value: URL or email of the designer.
- ```
  std::map<std::string, std::string>& Artists: get
  ```
    - The application artists.
    - Key: Name of the artist.
    - Value: URL or email of the artist.
- ```
  std::string TranslatorCredits: get, set
  ```
    - The application translator credits

### Methods
- ```cpp
  std::vector<std::string> getTranslatorNames() const
  ```
    - Returns: The list of names of translators of the application

### Static Functions
- ```cpp
  std::vector<<std::string> convertUrlMapToVector(const std::map<std::string, std::string>& urls)
  ```
    - Accepts: A map with keys of type string and values of type string, urls.
    - Returns: A list of entries in the following format: `"{key} {value}"`.
    - Ex: `AppInfo::convertUrlMapToVector({ { "a", "a2" }, { "b", "b2" } })` will return `std::vector<std::string>{ "a a2", "b b2" }`.

## Aura
Description: An application base.

Interface: [aura.h](/include/aura.h)

Type: `class`

Path: `Nickvision::Aura::Aura`

### Member Variables
- ```
  Nickvision::Aura::AppInfo& AppInfo: get
  ```
  - The AppInfo object for the application

### Methods
- ```cpp
  T& getConfig<T>(const std::string& key)
  ```
  - Accepts: The string key of the config file, key.
  - Returns: A reference to the configuration object of type T with key key.
  - Note: T must be a type that derives from `Nickvision::Aura::ConfigurationBase`
  - Ex: `getConfig<Configuration>("config")` will return the `Configuration` object parsed from a `config.json` file on disk.

### Static Functions
- ```cpp
  Nickvision::Aura::Aura& init(const std::string& id, const std::string& name, const std::string& englishShortName)
  ```
  - Accepts: An application id, id, an application name, name, and an application english short name, englishShortName.
  - Returns: A reference to the newly initialized singleton `Aura` object.
  - Throws: `std::runtime_error` if creation of Aura object fails.
- ```cpp
  Nickvision::Aura::Aura& getActive()
  ```
  - Returns: The reference to the singleton `Aura` object.
  - Throws: `std::logic_error` if `Aura::init()` was not yet called.
- ```cpp
  std::string getEnvVar(const std::string& key)
  ```
  - Accepts: The name of the environment variable, key.
  - Returns: The value of the environment variable with name key.
  - Returns: An empty string if no environment variable with the name key was found.
  - Ex: `Aura::getEnvVar("PATH")` will return the value of the system's path variable.
- ```cpp
  bool setEnvVar(const std::string& key, const std::string& value)
  ```
  - Accepts: The name of the environment variable, key, and the value to set said variable to, value.
  - Returns: `true` if the environment variable of name key was set to value.
  - Returns: `false` if setting the environment variable failed.