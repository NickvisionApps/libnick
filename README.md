# libaura

<img width='96' height='96' alt='Logo' src='resources/logo.png'/>

**A cross-platform base for native Nickvision applications.**

libaura provides Nickvision apps with a common set of cross-platform (Windows and Linux) APIs for managing system and desktop app functionality such as network management, taskbar icons, translations, app updates, and more.

## Documentation

Documentation for this library and its modules can be found [here](/docs).

## Dependencies

The following are a list of dependencies and their versions used by libaura. 

These dependencies should also be installed on your system and linked to your project when using libaura as well, since some libaura headers reference these dependencies.

Following the [build instructions](#building-and-installation) below using conan, will ensure these dependencies and their proper versions are installed for your platform.

**However, note, as stated above, when linking to libaura in your project, you MUST also link to these dependencies to ensure proper linkage to libaura.**

### All Platforms
- boost/1.83.0
- gtest/1.14.0 (**Only if building test suite**)
- jsoncpp/1.9.5
- libcurl/8.4.0
- libgettext/0.22
- maddy/1.3.0
- sqlcipher/4.5.1

### Linux Only
The above dependencies must be installed, plus the following for linux systems:
- glib/2.78.1
- libsecret/0.20.5
- libuuid/1.0.3

## Building and Installation
libaura uses Conan package manager for resolving dependencies and CMake as its build system.

Ensure both `conan` and `cmake` are installed on your system before building and installing libaura.

A C++20 compiler is also required to build libaura.

### Windows 10/11
Visual Studio 2022 with C++ Desktop workload is required to be installed.

1. First, clone/download the repo.
1. From the `conan` folder within the repo, copy `conanfile.py` and `conanprofile-windows.txt` to the root directory of the repo.
1. Open a terminal and navigate to the repo's root directory.
1. Run `conan install conanfile.py --profile:host=conanprofile-windows.txt --profile:build=conanprofile-windows.txt -s compiler.cppstd=20 --build=missing`.
1. Once that command finishes, cd into the `build` folder.
1. From the `build` folder, run `cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="generators/conan_toolchain.cmake"`.
    - To skip building libaura's test suite, add `-DBUILD_TESTING="Off"` to the end of the command.
1. From the `build` folder, run ``cmake --build . --config Release`.
1. After these commands complete, libaura will be successfully built and its binaries can be found in the `Release` folder of the `build` folder.
1. To install libaura to the system, from the `build` folder, run `cmake --install . --prefix "PATH_TO_INSTALL_DIR"`.
    - Replace `PATH_TO_INSTALL_DIR` with the path to a folder to install libaura to. 
    - This is usually a dependencies folder set up by the programmer, added to the PATH variable, to allow linking to said dependencies.
1. If contributing to upstream, remove `conanfile.py` and `conanprofile-windows.txt` from the repo's root directory as to not accidentally add and commit them.

### Linux
1. First, clone/download the repo.
1. From the `conan` folder within the repo, copy `conansettings.yml`, `conanfile.py`, and `conanprofile-linux.txt` to the root directory of the repo.
1. Rename `conansettings.yml` to `settings_user.yml`.
1. Open a terminal and navigate to the repo's root directory.
1. Run `conan config install settings_user.yml`.
1. Run `conan install conanfile.py --profile:host=conanprofile-linux.txt --profile:build=conanprofile-linux.txt -s compiler.cppstd=20 --build=missing`.
1. Once that command finished, cd into the `build` folder.
1. From the `build` folder, run `cmake .. -DCMAKE_TOOLCHAIN_FILE="Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release`.
    - To skip building libaura's test suite, add `-DBUILD_TESTING="Off"` to the end of the command.
1. From the `build` folder, run `cmake --build`.
1. After these commands complete, libaura will be successfully built and its binaries can be found in the `Release` folder of the `build` folder.
1. To install libaura to the system, from the `build` folder, run `cmake --install . --prefix "PATH_TO_INSTALL_DIR"`.
    - Replace `PATH_TO_INSTALL_DIR` with the path to a folder to install libaura to. 
    - This is usually a dependencies folder set up by the programmer, added to the PATH variable, to allow linking to said dependencies.
    - On linux, `PATH_TO_INSTALL_DIR` would usually be `/usr`.
1. If contributing to upstream, remove `conanfile.py` and `conanprofile-windows.txt` from the repo's root directory as to not accidentally add and commit them.