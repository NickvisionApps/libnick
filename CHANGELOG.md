# Changelog

## 2024.6.4 (next)
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