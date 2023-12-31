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
    - Ex: `Aura::setEnvVar("AURA", "true")` will set `"$AURA:true"`.

## ConfigurationBase
Description: A base class for configuration files.

Interface: [configurationbase.h](/include/configurationbase.h)

Type: `class`

Path: `Nickvision::Aura::ConfigurationBase`

### Member Variables
- ```
  std::string Key: get
  ```
    - The key of the configuration file.

### Events
- ```
  Event<Nickvision::Aura::Events::EventArgs> Saved
  ```
    - Invoked when the configuration file is saved to disk

### Methods
- ```cpp
  ConfigurationBase(const std::string& key)
  ```
    - Constructs the ConfigurationBase, loading the file from disk
    - Accepts: The key of the configuration file, key
- ```cpp
  bool save()
  ```
    - Returns: `true` if the configuration file was saved to disk
    - Returns: `false` if saving to disk failed

### Creating Your Own Configuration Files
The purpose of `ConfigurationBase` is to act as a base when defining your own configuration objects that you would like to be saved and retrieved from disk.

Here are some key points when defining your own configuration objects:
- Your configuration object's constructor must take a `const std::string& key` parameter and pass it to `ConfigurationBase`'s constructor. 
    - Although you will not use key in your own implementation, it is required for `ConfigurationBase`'s functionality and will be filled-in by the `Aura::getConfig()` method.
- `ConfigurationBase` exposes a protected `m_json` object which you must use in your implementation of getting and storing variables of your configuration object. 
    - If this `m_json` object is not used, your configuration object will not be stored to disk correctly.
- You must explicitly call the `save` method on your configuration object when you want to save the configuration to disk. Writing to the `m_json` object is not enough to trigger saving the file on disk

Here is an example of a custom configuration object using `ConfigurationBase`:
```cpp
class AppConfig : public ConfigurationBase
{
public:
	AppConfig(const std::string& key) : ConfigurationBase(key) 
	{ 

	}

	int getPreviousCount() const
	{
        //0 is the default value of PreviousCount (i.e. if it does not exist in the file)
		return m_json.get("PreviousCount", 0).asInt();
	}

	void setPreviousCount(int count)
	{
		m_json["PreviousCount"] = count;
	}
};
```
This object can now be used by an Aura-based application:
```cpp
void onConfigSaved(const Nickvision::Aura::Events::EventArgs& e)
{
    std::cout << "Config saved to disk." << std::endl;
}

int main()
{
    Aura::init(...);
    AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
    config.saved() += onConfigSaved;
    if(config.getPreviousCount() > 0)
    {
        std::cout << config.getPreviousCount() << std::endl;
    }
    config.setPreviousCount(6);
    config.save(); //onConfigSaved will be invoked on success
}
```

## DependencyLocator
Description: Functions for working with dependencies.

Interface: [dependencylocator.h](/include/dependencylocator.h)

Type: `namespace`

Path: `Nickvision::Aura::DependencyLocator`

### Functions
- ```cpp
  const std::filesystem::path& find(std::string dependency)
  ```
    - Accepts: The name of a dependency to find, dependency.
    - Returns: The path of the dependency on disk if found.
    - Returns: An empty path if the dependency was not found on disk.
    - Ex: `DependencyLocator::find("cmd")` on Windows will return `C:\Windows\System32\cmd.exe`.
    - Ex: `DependencyLocator::find("bash")` on Linux will return `/usr/bin/bash`.

## EnumFlags
Description: Macros for working with enums to be used as flags.

Interface: [enumflags.h](/include/enumflags.h)

Type: `namespace`

### Macros
- ```cpp
  #define DEFINE_ENUM_FLAG_OPERATORS(T)
  ```
    - Accepts: The type name of an `enum class` to be used as flags, T.
    - Defines: Operators `~`, `|`, `&`, `^`, `|=`, `&=`, `^=` for T.
    - Note: T must be of type `enum class`.
    - Note: T's values must be separate by bits, i.e. values must be 1, 2, 4, 8, 16, and so on.
 
## InterProcessCommunicator
Description: An inter process communicator (server/client).

Interface: [interprocesscommunicator.h](/include/interprocesscommunicator.h)

Type: `class`

Path: `Nickvision::Aura::InterProcessCommunicator`

### Member Variables
- ```
  bool isServer: get
  ```
    - Whether or not this instance is an IPC server.
- ```
  bool isClient: get
  ```
    - Whether or not this instance is an IPC client.

### Events
- ```
  Event<Nickvision::Aura::Events::ParamEventArgs<std::vector<std::string>>> CommandReceived
  ```
    - Invoked when this IPC server instance received a command from a client instance.
    - Note: If `isClient()` is true, this event will never be invoked for that instance.

### Methods
- ```cpp
  InterProcessCommunicator()
  ```
    - Constructs an InterProcessCommunicator.
    - Note: If this is the first IPC instance for all processes of this AppInfo::id, this instance will become an IPC server.
    - Note: If this is not the first IPC instance, this instance will become an IPC client. 
    - Throws: `std::logic_error` if Aura::init() was not called yet.
    - Throws: `std::runtime_error` if the client or server IPC cannot be created.
- ```cpp
  ~InterProcessCommunicator()
  ```
    - Destructs an InterProcessCommunicator.
- ```cpp
  bool communicate(const std::vector<std::string>& args, bool exitIfClient = false)
  ```
    - Accepts: A command separated by args, args, and a boolean on whether or not to exit the process if this instance is an IPC client, exitIfClient.
    - Returns: `true` if command-line arguments were sent to a server instance.
    - Returns: `false` on failure to send arguments to a server instance
    - Note: If this instance is the running server and communicate was called on said instance, `CommandReceived` will still be triggered with the passed args.

### Performing Inter-Process Communication
libaura uses named-pipes on Windows and Unix Domain Sockets on Linux to establish inter process server and client communicators, while abstracting all of that away from the consumer in the easy to use `InterProcessCommunicator` API.

Upon creating an `InterProcessCommunicator` object, either `isServer()` or `isClient()` will return true depending on whether or not this instance is a server or client respectively. Server instances should register a callback to the `CommandReceived` event to be invoked when clients send commands to the server.

NOTE: Whether or not `InterProcessCommunicator` is a client or server, its object should be kept alive for the entire running of an application to avoid accidentally removing a process as the server instance if the object is destroyed midway of an application running.

Let's consider an example scenario for using the `InterProcessCommunicator`. Assume we have an application where we want the first instance to be considered the main, running instance. Assume that if other instances of said application are started, we want its arguments to be passed to the main instance and then have said other instances be closed. This will allow, for example, a GUI main instance to be manipulated via secondary CLI instances. 

Here's the code for this:
```cpp
int main(int argc, char*[] argv)
{
    Aura::init(...); //ensures AppInfo::id
    std::vector<std::string> modernArgs;
    for(int i = 0; i < argc; i++)
    {
        if(argv[i])
        {
            modernArgs.push_back({ argv[i] });
        }
    }
    InterProcessCommunicator ipc;
    ipc.commandReceived() += handleArguments; //handleArguments definition excluded
    ipc.communicate(modernArgs, true);
}
```
If this program is ran for the first time, ipc will be the server instance. `handleArguments` will be invoked after `communicate` with its own arguments, as `communicate` still invokes `CommandReceived` even if ipc is the server instance. 

If this program is ran not for the first time, its arguments will be sent to the first instance and this instance itself will close. The first instance's `handleArguments` function will be called as a result of `CommandReceived` being invoked by the ipc server receiving the command. 

## SystemDirectories
Description: Functions for working with system directories.

Interface: [systemdirectories.h](/include/systemdirectories.h)

Type: `namespace`

Path: `Nickvision::Aura::SystemDirectories`

### Functions
- ```cpp
  std::vector<std::filesystem::path> getPath()
  ```
    - Returns: The list of directory paths found in the system's PATH variable.
- ```cpp
  std::vector<std::filesystem::path> getConfig()
  ```
    - Returns: The list of directory paths found in the system's XDG_CONFIG_DIRS variable.
- ```cpp
  std::vector<std::filesystem::path> getData()
  ```
    - Returns: The list of directory paths found in the system's XDG_DATA_DIRS variable.

## UserDirectories
Description: Functions for working with user directories.

Interface: [userdirectories.h](/include/userdirectories.h)

Type: `namespace`

Path: `Nickvision::Aura::UserDirectories`

### Functions
- ```cpp
  std::filesystem::path getHome()
  ```
    - Returns: The path to the user's home directory.
    - Returns: `FOLDERID_Profile` on Windows, `$HOME` on Linux.
- ```cpp
  std::filesystem::path getConfig()
  ```
    - Returns: The path to the user's config directory.
    - Returns: `FOLDERID_RoamingAppData` on Windows, `XDG_CONFIG_HOME` on Linux.
- ```cpp
  std::filesystem::path getApplicationConfig()
  ```
    - Returns: The path to the application's config directory.
    - Returns: `getConfig() + Aura::getActive().getAppInfo().getName()`
- ```cpp
  std::filesystem::path getCache()
  ```
    - Returns: The path to the user's cache directory.
    - Returns: `FOLDERID_LocalAppData` on Windows, `XDG_CACHE_HOME` on Linux.
- ```cpp
  std::filesystem::path getApplicationCache()
  ```
    - Returns: The path to the application's cache directory.
    - Returns: `getCache() + Aura::getActive().getAppInfo().getName()`
- ```cpp
  std::filesystem::path getLocalData()
  ```
    - Returns: The path to the user's local data directory.
    - Returns: `FOLDERID_RoamingAppData` on Windows, `XDG_DATA_HOME` on Linux.
- ```cpp
  std::filesystem::path getApplicationLocalData()
  ```
    - Returns: The path to the application's local data directory.
    - Returns: `getLocalData() + Aura::getActive().getAppInfo().getName()`
- ```cpp
  std::filesystem::path getRuntime()
  ```
    - Returns: The path to the user's runtime directory.
    - Returns: Empty path on Windows, `XDG_RUNTIME_DIR` on Linux.
- ```cpp
  std::filesystem::path getDesktop()
  ```
    - Returns: The path to the user's desktop directory.
    - Returns: `FOLDERID_Desktop` on Windows, `XDG_DESKTOP_DIR` on Linux.
- ```cpp
  std::filesystem::path getDocuments()
  ```
    - Returns: The path to the user's documents directory.
    - Returns: `FOLDERID_Documents` on Windows, `XDG_DOCUMENTS_DIR` on Linux.
- ```cpp
  std::filesystem::path getDownloads()
  ```
    - Returns: The path to the user's downloads directory.
    - Returns: `FOLDERID_Downloads` on Windows, `XDG_DOWNLOAD_DIR` on Linux.
- ```cpp
  std::filesystem::path getMusic()
  ```
    - Returns: The path to the user's music directory.
    - Returns: `FOLDERID_Music` on Windows, `XDG_MUSIC_DIR` on Linux.
- ```cpp
  std::filesystem::path getPictures()
  ```
    - Returns: The path to the user's pictures directory.
    - Returns: `FOLDERID_Pictures` on Windows, `XDG_PICTURES_DIR` on Linux.
- ```cpp
  std::filesystem::path getPublicShare()
  ```
    - Returns: The path to the user's pictures directory.
    - Returns: Empty path on Windows, `XDG_PUBLICSHARE_DIR` on Linux.
- ```cpp
  std::filesystem::path getTemplates()
  ```
    - Returns: The path to the user's pictures directory.
    - Returns: `FOLDERID_Templates` on Windows, `XDG_TEMPLATES_DIR` on Linux.
- ```cpp
  std::filesystem::path getVideos()
  ```
    - Returns: The path to the user's pictures directory.
    - Returns: `FOLDERID_Videos` on Windows, `XDG_VIDEOS_DIR` on Linux.

## Version
Description: A model for a version number. Formatted in "major.minor.build-dev".

Interface: [version.h](/include/version.h)

Type: `class`

Path: `Nickvision::Aura::Version`

### Member Variables
- ```
  Nickvision::Aura::VersionType VersionType: get
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
  const std::string& toString() const
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

Interface: [versiontype.h](/include/versiontype.h)

Type: `enum class`

Path: `Nickvision::Aura::VersionType`

### Values
- ```cpp
  Stable = 0
  ```
    - Represents a stable version
- ```cpp
  Preview = 1
  ```
    - Represents a preview (dev) version