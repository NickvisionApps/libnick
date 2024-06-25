# Nickvision::App

This module contains types and functions for creating Nickvision applications.

## Table of Contents
- [AppInfo](#appinfo)
- [Aura](#aura)
- [ConfigurationBase](#configurationbase)
- [InterProcessCommunicator](#interprocesscommunicator)
- [WindowGeometry](#windowgeometry)

## AppInfo
Description: A model for the information about an application.

Interface: [appinfo.h](/include/app/appinfo.h)

Type: `class`

Path: `Nickvision::App::AppInfo`

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
  Nickvision::Version Version: get, set
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
    - Note: Calling `AppInfo::setSourceRepo(sourceRepo)` can return `false` if the `sourceRepo` param is not a valid formatted URL.
- ```
  std::string IssueTracker: get, set
  ```
    - The application issue tracker url.
    - Ex: `"https://github.com/nickvisionapps/application/issues"`
    - Note: Calling `AppInfo::setIssueTracker(issueTracker)` can return `false` if the `issueTracker` param is not a valid formatted URL.
- ```
  std::string SupportUrl: get, set
  ```
    - The application support url.
    - Ex: `"https://github.com/nickvisionapps/application/discussions"`
    - Note: Calling `AppInfo::setSupportUrl(supportUrl)` can return `false` if the `supportUrl` param is not a valid formatted URL.
- ```
  std::string HtmlDocsStore: get, set
  ```
    - The application html docs store url.
    - Ex: `"https://raw.githubusercontent.com/NickvisionApps/Tagger/main/NickvisionTagger.Shared/Docs/html/"`
- ```
  std::unordered_map<std::string, std::string>& ExtraLinks: get
  ```
    - Extra links for the application.
    - Key: Name of the link.
    - Value: URL of the link.
- ```
  std::unordered_map<std::string, std::string>& Developers: get
  ```
    - The application developers.
    - Key: Name of the developer.
    - Value: URL or email of the developer.
- ```
  std::unordered_map<std::string, std::string>& Designers: get
  ```
    - The application designers.
    - Key: Name of the designer.
    - Value: URL or email of the designer.
- ```
  std::unordered_map<std::string, std::string>& Artists: get
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
  std::vector<<std::string> convertUrlMapToVector(const std::unordered_map<std::string, std::string>& urls)
  ```
    - Accepts: A map with keys of type string and values of type string, urls.
    - Returns: A list of entries in the following format: `"{key} {value}"`.
    - Ex: `AppInfo::convertUrlMapToVector({ { "a", "a2" }, { "b", "b2" } })` will return `std::vector<std::string>{ "a a2", "b b2" }`.

## Aura
Description: An application base.

Interface: [aura.h](/include/app/aura.h)

Type: `class`

Path: `Nickvision::App::Aura`

### Member Variables
- ```
  Nickvision::App::AppInfo& AppInfo: get
  ```
    - The AppInfo object for the application
- ```
  InterProcessCommunicator& IPC: get
  ```
    - The application's inter process communicator
- ```
  const Logger& Logger: get
  ```
    - The application's logger

### Methods
- ```cpp
  bool init(const std::string& id, const std::string& name, const std::string& englishShortName, Logging:LogLevel logLevel)
  ```
    - Accepts: An application id, id, an application name, name, an application english short name, englishShortName, and the application log level, logLevel.
    - Returns: True if initialized, else false.
    - Throws: `std::runtime_error` if the gettext system fails to initialize.
    - Throws: `std::runtime_error` if unable to get the executable directory path.
    - Note: This also calls Localization::Gettext::init().
- ```cpp
  std::string getHelpUrl(const std::string& pageName)
  ``` 
    - Accepts: The name of the documentation page to get the help url for, pageName.
    - Returns: The url for the documentation page. This will be a yelp url for Linux and a website url for Windows and Linux snaps.
    - Note: HtmlDocsStore should be set for Aura::getActive()::getAppInfo(). For Nickvision apps, this will be: `https://raw.githubusercontent.com/NickvisionApps/SHORT_APP_NAME/main/APP_NAME.Shared/Docs/html`, but can be customized for any app.
- ```cpp
  T& getConfig<T>(const std::string& key)
  ```
    - Accepts: The string key of the config file, key.
    - Returns: A reference to the configuration object of type T with key key.
    - Throws: `std::invalid_argument` if key is empty
    - Note: T must be a type that derives from `Nickvision::ConfigurationBase`
    - Ex: `getConfig<Configuration>("config")` will return the `Configuration` object parsed from a `config.json` file on disk.
- ```cpp
  Notifications::NotifyIcon& getNotifyIcon(HWND hwnd)
  ```
    - Accepts: The window handle for the notify icon, hwnd.
    - Returns: The application's NotifyIcon.
    - Note: The hwnd only needs to be valid on the first call to establish the new NotifyIcon.
    - Note: The new NotifyIcon will be hidden by default.
    - Note: This function is only available on Windows.

### Static Functions
- ```cpp
  Nickvision::Aura& getActive()
  ```
    - Returns: The reference to the singleton `Aura` object.

## ConfigurationBase
Description: A base class for configuration files.

Interface: [configurationbase.h](/include/app/configurationbase.h)

Type: `class`

Path: `Nickvision::App::ConfigurationBase`

### Member Variables
- ```
  std::string Key: get
  ```
    - The key of the configuration file.

### Events
- ```cpp
  Event<Nickvision::Events::EventArgs> Saved
  ```
    - Invoked when the configuration file is saved to disk

### Methods
- ```cpp
  ConfigurationBase(const std::string& key)
  ```
    - Constructs the ConfigurationBase, loading the file from disk
    - Accepts: The key of the configuration file, key
    - Throws: `std::invalid_argument` if key is empty
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
using namespace Nickvision::App;

class AppConfig : public ConfigurationBase
{
public:
	AppConfig(const std::string& key) 
    : ConfigurationBase{ key } 
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
using namespace Nickvision::App;
using namespace Nickvision::Events;

int main()
{
    Aura::getActive().init(...);
    AppConfig& config{ Aura::getActive().getConfig<AppConfig>("config") };
    config.saved() += [](const EventArgs& e) { std::cout << "Config saved to disk." << std::endl; };
    if(config.getPreviousCount() > 0)
    {
        std::cout << config.getPreviousCount() << std::endl;
    }
    config.setPreviousCount(6);
    config.save(); //lambda will be invoked on success
}
```
 
## InterProcessCommunicator
Description: An inter process communicator (server/client).

Interface: [interprocesscommunicator.h](/include/app/interprocesscommunicator.h)

Type: `class`

Path: `Nickvision::App::InterProcessCommunicator`

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
- ```cpp
  Event<Nickvision::Events::ParamEventArgs<std::vector<std::string>>> CommandReceived
  ```
    - Invoked when this IPC server instance received a command from a client instance.
    - Note: If `isClient()` is true, this event will never be invoked for that instance.

### Methods
- ```cpp
  InterProcessCommunicator(const std::string& id)
  ```
    - Constructs an InterProcessCommunicator.
    - Accepts: The id of the ipc, id. id should be the same value for all instances of ipc that must talk to each other.
    - Note: If this is the first IPC instance for all processes of id, this instance will become an IPC server.
    - Note: If this is not the first IPC instance, this instance will become an IPC client.
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
libnick uses named-pipes on Windows and Unix Domain Sockets on Linux to establish inter process server and client communicators, while abstracting all of that away from the consumer in the easy to use `InterProcessCommunicator` API.

Upon creating an `InterProcessCommunicator` object, either `isServer()` or `isClient()` will return true depending on whether or not this instance is a server or client respectively. Server instances should register a callback to the `CommandReceived` event to be invoked when clients send commands to the server.

NOTE: Whether or not `InterProcessCommunicator` is a client or server, its object should be kept alive for the entire running of an application to avoid accidentally removing a process as the server instance if the object is destroyed midway of an application running.

Let's consider an example scenario for using the `InterProcessCommunicator`. Assume we have an application where we want the first instance to be considered the main, running instance. Assume that if other instances of said application are started, we want its arguments to be passed to the main instance and then have said other instances be closed. This will allow, for example, a GUI main instance to be manipulated via secondary CLI instances. 

Here's the code for this:
```cpp
using namespace Nickvision::App;

int main(int argc, char*[] argv)
{
    Aura::getActive().init(...);
    std::vector<std::string> modernArgs;
    for(int i = 0; i < argc; i++)
    {
        if(argv[i])
        {
            modernArgs.push_back({ argv[i] });
        }
    }
    InterProcessCommunicator& ipc{ Aura::getActuve().getIPC() };
    ipc.commandReceived() += [](const Events::ParamEventArgs<std::vector<std::string>>& args) { ... };
    ipc.communicate(modernArgs, true);
}
```
If this program is ran for the first time, ipc will be the server instance. `handleArguments` will be invoked after `communicate` with its own arguments, as `communicate` still invokes `CommandReceived` even if ipc is the server instance. 

If this program is ran not for the first time, its arguments will be sent to the first instance and this instance itself will close. The first instance's `handleArguments` function will be called as a result of `CommandReceived` being invoked by the ipc server receiving the command. 

## WindowGeometry
Description: A model of a window's geometry.

Interface: [windowgeometry.h](/include/app/windowgeometry.h)

Type: `class`

Path: `Nickvision::App::WindowGeometry`

### Member Variables
- ```
  long Width: get, set
  ```
    - The window's width.
- ```
  long Height: get, set
  ```
    - The window's height.
- ```
  bool IsMaximized: get, set
  ```
    - Whether or not the window is maximized.

### Methods
- ```cpp
  WindowGeometry()
  ```
    - Constructs a WindowGeometry.
    - Note: Uses default values: 800 width, 600 height, not maximized.
- ```cpp
  WindowGeometry(long width, long height, bool isMaximized)
  ```
    - Constructs a WindowGeometry.
    - Accepts: The window's width, width, the window's height, height, and whether or not the window is maximized, isMaximized.
- ```cpp
  WindowGeometry(HWND hwnd)
  ```
    - Constructs a WindowGeometry.
    - Accepts: The window handle to get the geometry from, hwnd.
    - Note: This function is only available on Windows.
- ```cpp
  bool apply(HWND hwnd)
  ```
    - Accepts: The window handle to apply the geometry to, hwnd.
    - Returns: True if successful.
    - Returns: False if unsuccessful.