# Changelog

## 2024.6.3 (next)
### Breaking Changes
#### App
- Removed `Nickvision::App::Aura::getEnvVar()`
- Removed `Nickvision::App::Aura::setEnvVar()`
- Removed `Nickvision::App::Aura::sysExec()`
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

## 2024.6.2
### Breaking Changes
None
### New APIs
None
### Fixes
#### Update
- Improved `Nickvision::Update::Updater` API