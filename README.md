# libnick

<img width='96' height='96' alt='Logo' src='resources/libnick-r.svg'/>

**A cross-platform base for native Nickvision applications.**

libnick provides Nickvision apps with a common set of cross-platform APIs for managing system and desktop app functionality such as network management, taskbar icons, translations, app updates, and more.

## Documentation

Documentation for this library and its modules can be found [here](https://nickvision.org/docs/libnick/index.html).

## Dependencies
The following are a list of dependencies used by libnick. 

The recommendation is to (and below commands will) use vcpkg to pull these dependencies. However, vcpkg is not a requirement as long as the system provides these dependencies correctly.

### All Platforms
- boost-json
- cpr
- gtest
- libintl
- maddy
- sqlcipher (Must be provided by the system on Linux and macOS as sqlcipher is not available via vcpkg for these systems)

### Linux and macOS
The above dependencies must be installed, *plus* the following for Linux and macOS systems:
- glib
- libsecret
  - For macOS: Only required if `-DUSE_LIBSECRET="ON"`

## Consuming libnick via vcpkg
libnick is available through `vcpkg`. 

Simply install and configure vcpkg for your system, and run:
```
vcpkg install libnick
```
You can then use libnick from cmake with the following:
```
find_package(libnick CONFIG REQUIRED)
target_link_libraries(main PRIVATE libnick::libnick)
```

## Building and Installing Manually
libnick uses `cmake` as its build system and `vcpkg` to *optionally* manage its dependencies.

Ensure `cmake` and `vcpkg` are installed on your system before building and installing libnick.

A C++20 compiler is also required to build libnick.

### Configuring vcpkg
1. Set the `VCPKG_ROOT` environment variable to the path of your vcpkg installation's root directory.
#### Windows
1. Set the `VCPKG_DEFAULT_TRIPLET` environment variable to `x64-windows`
1. Run `vcpkg install boost-json cpr gettext-libintl gtest maddy sqlcipher`
#### Linux
1. Set the `VCPKG_DEFAULT_TRIPLET` environment variable to `x64-linux`
1. Run `vcpkg install boost-json cpr gettext-libintl glib gtest libsecret maddy`
#### macOS (Apple Silicon)
1. Set the `VCPKG_DEFAULT_TRIPLET` environment variable to `arm64-osx`
1. Run `vcpkg install boost-json cpr gettext-libintl glib gtest libsecret maddy`

### Building
1. First, clone/download the repo.
1. Open a terminal and navigate to the repo's root directory.
1. Create a new `build` directory and `cd` into it. 
#### Windows
1. From the `build` folder, run `cmake .. -G "Visual Studio 17 2022"`.
    - To skip building libnick's test suite, add `-DBUILD_TESTING="OFF"` to the end of the command.
    - If you plan to install libnick, add `-DCMAKE_INSTALL_PREFIX=PATH_TO_INSTALL_DIR` to the end of the command, replacing `PATH_TO_INSTALL_DIR` with the path of where you'd like libnick to install to.
1. From the `build` folder, run `cmake --build . --config Release`.
1. After these commands complete, libnick will be successfully built and its binaries can be found in the `Release` folder of the `build` folder.
#### Linux
1. From the `build` folder, run `cmake .. -DCMAKE_BUILD_TYPE=Release`.
    - To skip building libnick's test suite, add `-DBUILD_TESTING="OFF"` to the end of the command.
    - If you plan to install libnick, add `-DCMAKE_INSTALL_PREFIX=PATH_TO_INSTALL_DIR` to the end of the command, replacing `PATH_TO_INSTALL_DIR` with the path of where you'd like libnick to install to.
1. From the `build` folder, run `cmake --build .`.
1. After these commands complete, libnick will be successfully built and its binaries can be found in the `build` folder.
#### macOS
1. From the `build` folder, run `cmake .. -DCMAKE_BUILD_TYPE=Release`.
    - To skip building libnick's test suite, add `-DBUILD_TESTING="OFF"` to the end of the command.
    - To use `libsecret` instead of macOS's built in security library, add `-DUSE_LIBSECRET="ON"` to the end of the command.
    - If you plan to install libnick, add `-DCMAKE_INSTALL_PREFIX=PATH_TO_INSTALL_DIR` to the end of the command, replacing `PATH_TO_INSTALL_DIR` with the path of where you'd like libnick to install to.
1. From the `build` folder, run `cmake --build .`.
1. After these commands complete, libnick will be successfully built and its binaries can be found in the `build` folder.

### Installing
1. To install libnick to the system, from the `build` folder, run `cmake --install .`.
    - This command will export and install libnick cmake targets allowing you to simply use libnick in other cmake projects with the following:
        ```
        find_package(libnick CONFIG REQUIRED)
        target_link_libraries(main PRIVATE libnick::libnick)
        ```
