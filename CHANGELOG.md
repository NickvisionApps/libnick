# Changelog

## 2025.9.4
### Breaking Changes
None
### New APIs
#### Database
- Added `reset` method to `SqliteStatement`
### Fixes
#### Database
- Fixed an issue where the sqlite database crashed on Windows

## 2025.9.3
### Breaking Changes
None
### New APIs
None
### Fixes
#### Database
- Fixed an issue where the sqlite database did not encrypt and decrypt correctly

## 2025.9.2
### Breaking Changes
None
### New APIs
None
### Fixes
#### Helpers
- Fixed a deadlock in `JsonFileBase`

## 2025.9.1
### Breaking Changes
None
### New APIs
None
### Fixes
#### Helpers
- Fixed an issue where `JsonFileBase::get()` did not work for `std::string`

## 2025.9.0
### Breaking Changes
#### App
- Removed `DataFileBase`
- Removed `DataFileManager`
- Moved `AppInfo::convertUrlMapToVector()` to `Helpers::CodeHelpers` namespace
- Moved `App::CancellationToken` to `Helpers` namespace
#### Database
- Rewrote all classes with a more modern interface
- Renamed all classes with `Sqlite` prefix
#### Keyring
- Rewrote `Keyring` class with a more modern interface
- Moved `SystemCredentials` to `System::Credentials` namespace
- Removed `CredentialCheckStatus` enum
- Removed `PasswordStrength` module
#### Network
- Removed `DNS` module
- Removed `Socket` and related classes
#### Taskbar
- Removed module
### New APIs
#### Helpers
- Added `DEFINE_ENUM_FLAGS()` macro to `CodeHelpers` namespace
- Added `IJsonSerializable` interface
- Added `JsonFileBase` to replace `DataFileBase`
- Added `quote()` function to `StringHelpers` namespace
#### Keyring
- Added `Space` value to `PasswordContent`
#### System
- Added `execAsync()` function to `Environment` namespace 
### Fixes
- Marked functions and methods with `noexcept` where applicable
#### System
- Fixed `Process`'s handling of quoted strings

## 2025.8.0
### Breaking Changes
None
### New APIs
#### App
- Added `reset()` method to `CancellationToken`
### Fixes
#### Events
- Fixed the `operator->()` method for `ParamEventArgs`

## 2025.7.6
### Breaking Changes
None
### New APIs
None
### Fixes
#### Updater
- Exceptions thrown

## 2025.7.5
### Breaking Changes
None
### New APIs
#### Update
- Added `Updater::downloadUpdate()` method for all platforms
### Fixes
None

## 2025.7.4
### Breaking Changes
- `maddy` dependency requires >= 1.6.0
### New APIs
#### App
- Added `CancellationToken` class
### Fixes
#### Localization
- Fixed included headers
- Fixed Gettext::changeLanguage("C") not turning off translations

## 2025.7.3
### Breaking Changes
None
### New APIs
#### Localization
- You can now specify "C" in the `Gettext::changeLanguage()` function to turn off translations.
### Fixes
#### Localization
- Improved `Gettext::getAvailableLanguages()`'s search for languages

## 2025.7.2
### Breaking Changes
None
### New APIs
#### Localization
- Added `Gettext::changeLanguage()` function
### Fixes
None

## 2025.7.1
### Breaking Changes
None
### New APIs
None
### Fixes
#### Helpers
- Fixed an issue where `StringHelpers::isValidUrl()` did not work correctly

## 2025.7.0
### Breaking Changes
#### App
- Removed `App:InterProcessCommunicator` (can be easily recreated using the `Network::Socket` class)
#### Logging
- Removed module
#### Network
- Replaced dependency of `libcurl` with `cpr`
- Renamed `Web::fetchJson()` to `Web::getJson()`
- Removed `CurlEasy` class
### New APIs
#### App
- Added `isPortable` parameter to `DataFileManager` to store config files in the executable directory instead
#### Helpers
- Added `PairHash` class
- Added `CodeHelpers::combineHash()` function
#### System
- Added `DependencySearchOption` enum
- Added `search` parameter to `Environment::findDependency()` to specify a `DependencySearchOption`
### Fixes
#### System
- Fixed `Process::pause()` and `Process::resume()` not working

## 2025.6.5
### Breaking Changes
None
### New APIs
None
### Fixes
- Fixed an issue where the build failed on Unix systems

## 2025.6.4
### Breaking Changes
None
### New APIs
None
### Fixes
#### System
- Fixed an issue where `Process::pause()` and `Process::resume()` did not work correctly on Windows

## 2025.6.3
### Breaking Changes
None
### New APIs
None
### Fixes
- Fixed an issue with the cmake configuration file

## 2025.6.2
### Breaking Changes
#### Database
- sqlcipher is no longer provided for Linux and macOS builds. The system must provide it via it's package manager as it is not available via vcpkg
### New APIs
#### Update
- Added a `CurlFunctionParameter` parameter to `Updater::windowsUpdate` 
### Fixes
None

## 2025.6.1
### Breaking Changes
None
### New APIs
#### App
- Expanded `X` and `Y` properties of `WindowGeometry` to all platforms
### Fixes
None

## 2025.6.0
### Breaking Changes
None
### New APIs
#### App
- Added the ability for `WindowGeometry` to remember window position on Windows
### Fixes
None

## 2025.5.3
### Breaking Changes
#### Localization
- `_f()` and `_fn()` macros now return `std::string` instead of `const char*`
### New APIs
None
### Fixes
#### System
- Fixed `Process::pause()` and `Process::resume()` not working on Windows

## 2025.5.2
### Breaking Changes
None
### New APIs
#### Localization
- Added `_fn()` macro for creating strings with translated plural format strings
### Fixes
- Fixed compilation on x86 Windows

## 2025.5.1
### Breaking Changes
#### System
- A Process' status can now be queued with `Process::getState()` which returns a `ProcessState` value
### New APIs
#### Localization
- Added `_f()` macro for creating strings with translated format strings
- Added `Gettext::getAvailableLanguages()` function
#### System
- Added `ProcessState` enum
- Added `Process::pause()` method
- Added `Process::resume()` method
### Fixes
None

## 2025.5.0
### Breaking Changes
#### Notifications
- Removed dependency of `wintoast` on Windows
### New APIs
#### System
- Added `Environment::getExecutablePath()` function
### Fixes
#### Notifications
- Fixed an issue where `ShellNotifications::send()` did not work on Windows

## 2025.3.6
### Breaking Changes
None
### New APIs
None
### Fixes
- Library linking on Windows

## 2025.3.5
### Breaking Changes
#### Notifications
- Standardized `ShellNotifications::send()` to take the same parameters across platforms
- Removed `NotifyIcon` as UI platform should be used instead
### New APIs
#### Notifications
- Added `AppNotification` namespace to allow applications to send app notifications anywhere
### Fixes
None

## 2025.3.4
### Breaking Changes
None
### New APIs
#### Events
- Added `*` and `->` operator support to `ParamEventArgs`
### Fixes
- Fixed an issue where could not compiler on older macOS systems
#### System
- Fixed an issue where `Process::getRAMUsage()` did not account for child processes on Windows
- Improved the implementation of `Process::getCPUUsage()` on Windows

## 2025.3.3
### Breaking Changes
None
### New APIs
#### Helpers
- Added `includeEmpty` parameter to `StringHelpers::split()` function
#### System
- Added `Process::getRAMUsage()` function
- Added `Process::getCPUUsage()` function
### Fixes
#### Helpers
- Improved `StringHelpers::split()` implementation

## 2025.3.2
### Breaking Changes
None
### New APIs
#### System
- Added `Process::send()` function
- Added `Process::sendCommand()` function
### Fixes
None

## 2025.3.1
### Breaking Changes
None
### New APIs
None
### Fixes
#### System
- Fixed overflow in `HardwareInfo` functions

## 2025.3.0
### Breaking Changes
None
### New APIs
#### System
- Modified `Nickvision::System::Process`'s constructor to now take an optional working directory parameter
### Fixes
#### Helpers
- Implemented non-deprecated algorithms for `StringHelpers::str()` and `StringHelpers::wstr()`
#### Network
- Improved user agent string used by `Nickvision::Web::fetchJsonString()`

## 2025.2.0
### Breaking Changes
None
### New APIs
#### Helpers
- Added `Nickvision::Helpers::SizeHelpers` namespace
#### System
- Added `Nickvision::System::HardwareInfo` namespace
### Fixes
None

## 2025.1.0
### Breaking Changes
None
### New APIs
None
### Fixes
#### System
- Fixed an issue where `Process::kill()` did not work on Windows

## 2024.12.1
### Breaking Changes
None
### New APIs
#### Network
- Added `Nickvision::Network::Socket` class
### Fixes
None

## 2024.12.0
### Breaking Changes
None
### New APIs
None
### Fixes
#### Notifications
- Fixed an issue where `ShellNotification::send()` did not work on non-GTK linux applications
#### System
- Fixed an issue where `Environment::getExecutableDirectory()` did not return the correct path on macOS

## 2024.11.1
### Breaking Changes
None
### New APIs
None
### Fixes
- Fixed compilation issues for older macOS systems

## 2024.11.0
### Breaking Changes
- Added the `USE_LIBSECRET` cmake option that can be specified on macOS systems to use `libsecret` for credential management instead of macOS Keychain APIs
### New APIs
None
### Fixes
- Fixed compilation issues for older macOS systems

## 2024.10.0
### Breaking Changes
None
### New APIs
None
### Fixes
#### Localization
- Fixed an issue where translated strings did not show UTF-8 characters correctly on Windows

## 2024.9.2
### Breaking Changes
None
### New APIs
#### System
- Added `Nickvision::System::Environment::hasVariable()`
### Fixes
#### System
- Fixed an issue where Flatpak deployment mode was not correctly detected

## 2024.9.1
### Breaking Changes
None
### New APIs
None
### Fixes
### App
- Fixed an issue where `DataFileBase` was incorrectly parsing JSON files

## 2024.9.0
### Breaking Changes
- Replaced `jsoncpp` library with `boost-json`
#### App
- `Nickvision::App::DataFileBase`'s `m_json` object is now of type `boost::json::object`
### New APIs
None
### Fixes
None

## 2024.8.3
### Breaking Changes
#### Keyring
- The `Nickvision::Keyring::Keyring` class has been rewritten for better performance and a cleaner API. Keyrings created with previous versions of libnick are no longer compatible.
- Removed `Nickvision::Keyring::KeyringDialogController`
- Removed `Nickvision::Keyring::Store`
### New APIs
None
### Fixes
None

## 2024.8.2
### Breaking Changes
None
### New APIs
None
### Fixes
#### System
- Improved `Nickvision::System::Process` implementation on UNIX systems

## 2024.8.1
### Breaking Changes
None
### New APIs
None
### Fixes
#### System
- Improved `Nickvision::System::Process`'s handling of arguments

## 2024.8.0
### Breaking Changes
#### System
- `Nickvision::System::Process::kill()` will now kill child processes spawned by the respective process
### New APIs
None
### Fixes
#### Logging
- Cleaned up the message logged by `Nickvision::Logging::Logger:log()`
#### System
- Improved `Nickvision::System::Process`'s handling of arguments

## 2024.7.3
### Breaking Changes
- Removed the dependency on `libuuid`
#### Database
- Non-constructor methods will no longer throw exceptions but return true or false upon success.
#### Network
- Non-constructor methods will no longer throw exceptions but return true or false upon success.
- Removed `Nickvision::Network::WebClient` class
### New APIs
#### Events
- Added `Nickvision::Events::Event::count()` method
- Added `Nickvision::Events::Event::operator bool()` method
#### Helpers
- Added `Nickvision::Helpers::StringHelpers::newUuid()` method
#### Network
- Added `Nickvision::Network::DNS` namespace
- Added `Nickvision::Network::IPv4Address` class
- Added `Nickvision::Network::MacAddress` class
- Added `Nickvision::Network::Web` class
### Fixes
#### Helpers
- Improved GUID/UUID generation in `Nickvision::Helpers::StringHelpers`

## 2024.7.2
### Breaking Changes
None
### New APIs
#### System
- Added `Nickvision::System::Environment::testVariable()`
### Fixes
#### Network
- Fixed a deadlock that can occur when reading `Nickvision::Network::NetworkMonitor`'s connectionState

## 2024.7.1
### Breaking Changes
#### Helpers
- The default value of `Nickvision::Helpers::StringHelpers::join`'s `separateLast` parameter is now `false`
### New APIs
None
### Fixes
#### System
- Fixed an issue where `Nickvision::System::SuspendInhibitor` did not work correctly on Linux systems

## 2024.7.0
### Breaking Changes
#### Filesystem
- Removed application directories from `Nickvision::Filesystem::UserDirectory` enum
### New APIs
#### Filesystem
- Added `Nickvision::Filesystem::ApplicationUserDirectory` enum
- Added `Nickvision::Filesystem::UserDirectories::get()` function overload for `Nickvision::Filesystem::ApplicationUserDirectory` support
#### Helpers
- Added `Nickvision::Helpers::StringHelpers::normalizeForFilename()`
- Added `Nickvision::Helpers::StringHelpers::replace()` character overload
### Fixes
#### Update
- Fixed an issue where `Nickvision::Update::Updater::windowsUpdate()` did not work correctly

## 2024.6.9
### Breaking Changes
#### App
- Removed `Nickvision::App::Aura`
- Renamed `Nickvision::App::ConfigurationBase` to `Nickvision::App::DataFileBase`
#### Filesystem
- Removed `Nickvision::Filesystem::SystemDirectories` namespace
- Removed `Nickvision::Filesystem::UserDirectories::get*` methods
### New APIs
#### App
- Added `Nickvision::App::DataFileManager` class
#### Filesystem
- Added `Nickvision::Filesystem::UserDirectory` enum
- Added `Nickvision::Filesystem::UserDirectories::get()`
#### Notifications
- Added `Nickvision::Notifications::NotifyIcon::getForHWND()`
#### Localization
- Added `Nickvision::Localization::Documentation::getHelpUrl()`
#### System
- Added `Nickvision::System::DeploymentMode` enum
- Added `Nickvision::System::OperatingSystem` enum
- Added `Nickvision::System::Environment::getOperatingSystem()`
- Added `Nickvision::System::Environment::getDeploymentMode()`
- Added `Nickvision::System::Environment::getExecutableDirectory()`
- Added `Nickvision::System::Environment::getLocaleName()`
- Added `Nickvision::System::Environment::getPath()`
- Added `Nickvision::System::Environment::findDependency()`
### Fixes
#### Filesystem
- `Nickvision::Filesystem::UserDirectories` will now fetch proper `macOS` directories
#### Localization
- `Nickvision::Localization::Gettext::init()` will ensure the domainName is lowercase and contains no spaces
#### Update
- Fixed an issue where Updater would crash if failed to fetch json

## 2024.6.8
### Breaking Changes
None
### New APIs
None
### Fixes
- Fixed x86 Windows builds

## 2024.6.7
### Breaking Changes
None
### New APIs
- Added support for `macOS` across the library
#### Helpers
- `Nickvision::Helpers::StringHelpers::splitArgs()`
#### System
- `Nickvision::System::SuspendInhibitor` class
### Fixes
#### System
- `Nickvision::System::Environment::exec()` now correctly handles quotes args

## 2024.6.6
### Breaking Changes
#### App
- `Nickvision::App::Aura` now disables move operations, as well as previously disabled copy operations
#### Events
- `Nickvision::Events::HandlerId` is now defined as `enum class HandlerId : size_t` instead of `using HandlerId = size_t`
#### Helpers
- `Nickvision::Helpers::StringHelpers::encode` now accepts `std::vector<std::byte>` instead of `std::vector<std::uint_8>`
- `Nickvision::Helpers::StringHelpers::decode` now returns `std::vector<std::byte>` instead of `std::vector<std::uint_8>`
#### Notifications
- `Nickvision::Notifications::NotifyIcon` constructor now requires specification of the tooltip string
### New APIs
#### App
- `Nickvision::App::Aura::getNotifyIcon()`
#### Helpers
- `Nickvision::Helpers::CodeHelpers::readFileBytes()`
- `Nickvision::Helpers::CodeHelpers::writeFileBytes()`
#### Notifications
- `Nickvision::Notifications::NotifyIcon::getContextMenu()`
- `Nickvision::Notifications::NotifyIcon::setContextMenu()`
### Fixes
- Enum flag operators are now `constexpr` where applicable

## 2024.6.5
### Breaking Changes
None
### New APIs
None
### Fixes
- Fixed cmake config file

## 2024.6.4
### Breaking Changes
#### App
- `Nickvision::App::Aura::init()` will no longer initialize `libcurl`
#### Helpers
- Renamed `Nickvision::CodeHelpers` namespace to `Nickvision::Helpers::CodeHelpers`
- Renamed `Nickvision::StringHelpers` namespace to `Nickvision::Helpers::StringHelpers`
- Removed `Nickvision::WebHelpers` namespace
### New APIs
#### Helpers
- `Nickvision::Helpers::CodeHelpers::getLastSystemError()`
#### Network
- `Nickvision::Network::CurlEasy` class
- `Nickvision::Network::WebClient` class
### Fixes
None

## 2024.6.3
### Breaking Changes
- Removed `enumflags.h`
- Added `sqlcipher` as a dependency for Windows build
  - `sqlcipher` will continue to be built manually by libnick on Linux until the `vcpkg` port is fixed for Linux
#### App
- Removed `Nickvision::App::Aura::getEnvVar()`
- Removed `Nickvision::App::Aura::setEnvVar()`
- Removed `Nickvision::App::Aura::sysExec()`
#### Events
- `Nickvision::Events::Event::subscribe()` and `Nickvision::Events::Event::operator+=()`will now return a `Nickvision::Events::HandlerId` that must be passed to `Nickvision::Events::Event::unsubscribe()` and `Nickvision::Events::Event::operator-=()` to remove a handler.
#### Localization
- Removed `Nickvision::Localization::Documentation` namespace
#### StringHelpers
- `Nickvision::StringHelpers::split()` will return a list with an empty string when the string to split is empty, instead of an empty list
- Renamed `Nickvision::StringHelpers::toBase64()` to `Nickvision::StringHelpers::encode()`
- Renamed `Nickvision::StringHelpers::toByteList()` to `Nickvision::StringHelpers::decode()`
- Renamed `Nickvision::StringHelpers::toLower()` to `Nickvision::StringHelpers::lower()`
- Renamed `Nickvision::StringHelpers::toString()` to `Nickvision::StringHelpers::str()`
- Renamed `Nickvision::StringHelpers::toUpper()` to `Nickvision::StringHelpers::upper()`
- Renamed `Nickvision::StringHelpers::toWstring()` to `Nickvision::StringHelpers::wstr()`
#### Update
- Renamed `Nickvision::Update::Version::toString()` to `Nickvision::Update::Version::str()`
### New APIs
#### App
- `Nickvision::App::Aura::isValid()`
- `Nickvision::App::Aura::getHelpUrl()`
- `Nickvision::App::Aura::operator bool()`
#### System
- `Nickvision::System::Environment` namespace
  - Provides `getVariable()`
  - Provides `setVariable()`
  - Provides `clearVariable()`
  - Provides `sysExec()`
- `Nickvision::System::Process` class
- `Nickvision::System::ProcessExitedEventArgs` class
### Fixes
#### Keyring
- Fixed an issue where `Nickvision::Keyring::SystemCredentials::addCredential` freed memory incorrectly on Windows
- Improved the memory usage of `Nickvision::Network::NetworkMonitor` on Windows
#### Tests
- Improved test code

## 2024.6.2
### Breaking Changes
None
### New APIs
None
### Fixes
#### Update
- Improved `Nickvision::Update::Updater` API
