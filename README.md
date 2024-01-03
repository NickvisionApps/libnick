# libaura

A cross-platform base for native Nickvision applications.

libaura provides Nickvision apps with a common set of cross-platform (Windows and Linux) APIs for managing system and desktop app functionality such as network management, taskbar icons, translations, app updates, and more.

## Documentation

Documentation for this library and its modules can be found [here](/docs).

## Building and Installing
libaura uses `vcpkg` to manage its dependencies and `cmake` as its build system.

### Configuring vcpkg
Once vcpkg is installed on your system: 
1. Set the `VCPKG_ROOT` environment variable to the path of your vcpkg installation's root directory.
#### Windows
1. Set `VCPKG_DEFAULT_TRIPLET` to `x64-windows`
1. Run `vcpkg install boost-locale curl gettext gettext-libintl gtest jsoncpp maddy sqlcipher`
#### Linux
1. Set `VCPKG_DEFAULT_TRIPLET` to `x64-linux`
1. Run `vcpkg install boost-locale curl gettext gettext-libintl glib gtest jsoncpp libsecret libuuid maddy sqlcipher`

### Building libaura