cmake_minimum_required (VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH $ENV{VCPKG_ROOT} VCPKG_ROOT)
    if(EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(USING_VCPKG ON)
    endif()
endif()
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#libnick Definition
project ("libnick" LANGUAGES C CXX VERSION 2025.9.3 DESCRIPTION "A cross-platform base for native Nickvision applications.")
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CTest)

if(APPLE)
    option(USE_LIBSECRET "Use libsecret on macOS instead of Apple keychain" OFF)
endif()

#libnick Setup
add_compile_definitions(SQLITE_HAS_CODEC)
if(NOT WIN32)
    add_compile_definitions(HAVE_USLEEP)
endif()
add_library (${PROJECT_NAME}
    "include/app/appinfo.h"
    "include/app/windowgeometry.h"
    "include/database/sqlite.h"
    "include/database/sqlitedatabase.h"
    "include/database/sqlitefunctioncontext.h"
    "include/database/sqlitestatement.h"
    "include/database/sqlitevalue.h"
    "include/events/event.h"
    "include/events/eventargs.h"
    "include/events/parameventargs.h"
    "include/filesystem/applicationuserdirectory.h"
    "include/filesystem/fileaction.h"
    "include/filesystem/filesystemchangedeventargs.h"
    "include/filesystem/filesystemwatcher.h"
    "include/filesystem/userdirectories.h"
    "include/filesystem/userdirectory.h"
    "include/filesystem/watcherflags.h"
    "include/helpers/cancellationtoken.h"
    "include/helpers/codehelpers.h"
    "include/helpers/ijsonserializable.h"
    "include/helpers/jsonfilebase.h"
    "include/helpers/pairhash.h"
    "include/helpers/stringhelpers.h"
    "include/keyring/credential.h"
    "include/keyring/keyring.h"
    "include/keyring/passwordcontent.h"
    "include/keyring/passwordgenerator.h"
    "include/localization/documentation.h"
    "include/localization/gettext.h"
    "include/network/ipv4address.h"
    "include/network/macaddress.h"
    "include/network/networkmonitor.h"
    "include/network/networkstate.h"
    "include/network/networkstatechangedeventargs.h"
    "include/network/web.h"
    "include/notifications/appnotification.h"
    "include/notifications/notificationsenteventargs.h"
    "include/notifications/notificationseverity.h"
    "include/notifications/shellnotification.h"
    "include/notifications/shellnotificationsenteventargs.h"
    "include/system/credentials.h"
    "include/system/dependencysearchoption.h"
    "include/system/deploymentmode.h"
    "include/system/environment.h"
    "include/system/hardwareinfo.h"
    "include/system/operatingsystem.h"
    "include/system/process.h"
    "include/system/processexitedeventargs.h"
    "include/system/processstate.h"
    "include/system/suspendinhibitor.h"
    "include/update/updater.h"
    "include/update/version.h"
    "include/update/versiontype.h"
    "src/app/appinfo.cpp"
    "src/app/windowgeometry.cpp"
    "src/database/sqlitedatabase.cpp"
    "src/database/sqlitefunctioncontext.cpp"
    "src/database/sqlitestatement.cpp"
    "src/database/sqlitevalue.cpp"
    "src/filesystem/filesystemchangedeventargs.cpp"
    "src/filesystem/filesystemwatcher.cpp"
    "src/filesystem/userdirectories.cpp"
    "src/helpers/cancellationtoken.cpp"
    "src/helpers/codehelpers.cpp"
    "src/helpers/jsonfilebase.cpp"
    "src/helpers/stringhelpers.cpp"
    "src/keyring/credential.cpp"
    "src/keyring/keyring.cpp"
    "src/keyring/passwordgenerator.cpp"
    "src/localization/documentation.cpp"
    "src/localization/gettext.cpp"
    "src/network/ipv4address.cpp"
    "src/network/macaddress.cpp"
    "src/network/networkmonitor.cpp"
    "src/network/networkstatechangedeventargs.cpp"
    "src/network/web.cpp"
    "src/notifications/appnotification.cpp"
    "src/notifications/notificationsenteventargs.cpp"
    "src/notifications/shellnotification.cpp"
    "src/notifications/shellnotificationsenteventargs.cpp"
    "src/system/credentials.cpp"
    "src/system/environment.cpp"
    "src/system/hardwareinfo.cpp"
    "src/system/process.cpp"
    "src/system/processexitedeventargs.cpp"
    "src/system/suspendinhibitor.cpp"
    "src/update/updater.cpp"
    "src/update/version.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_VERSION}")
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING "${PROJECT_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

#libnick Packages
find_package(Boost REQUIRED COMPONENTS json)
find_package(cpr CONFIG REQUIRED)
find_package(Intl REQUIRED)
find_package(maddy CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::json cpr::cpr Intl::Intl maddy::maddy)
if(WIN32)
    find_package(sqlcipher CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC sqlcipher::sqlcipher Advapi32 Dnsapi Dwmapi Gdiplus Kernel32 Shell32 UxTheme)
elseif(APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_library(CF_LIBRARY CoreFoundation)
    find_library(CS_LIBRARY CoreServices)
    find_library(IOKIT_LIBRARY IOKit)
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(sqlcipher REQUIRED IMPORTED_TARGET sqlcipher)
    pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
    pkg_check_modules(gio REQUIRED IMPORTED_TARGET gio-2.0)
    pkg_check_modules(gmodule REQUIRED IMPORTED_TARGET gmodule-2.0)
    pkg_check_modules(gobject REQUIRED IMPORTED_TARGET gobject-2.0)
    pkg_check_modules(gthread REQUIRED IMPORTED_TARGET gthread-2.0)
    if(USE_LIBSECRET)
        pkg_check_modules(libsecret REQUIRED IMPORTED_TARGET libsecret-1)
        add_compile_definitions(APPLE_USE_LIBSECRET)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${CF_LIBRARY} ${CS_LIBRARY} ${IOKIT_LIBRARY} Threads::Threads PkgConfig::sqlcipher PkgConfig::glib PkgConfig::gio PkgConfig::gmodule PkgConfig::gobject PkgConfig::gthread PkgConfig::libsecret)
    else()
        find_library(SECURITY_LIBRARY Security)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${CF_LIBRARY} ${CS_LIBRARY} ${IOKIT_LIBRARY} ${SECURITY_LIBRARY} Threads::Threads PkgConfig::sqlcipher PkgConfig::glib PkgConfig::gio PkgConfig::gmodule PkgConfig::gobject PkgConfig::gthread)
    endif()
else()
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(sqlcipher REQUIRED IMPORTED_TARGET sqlcipher)
    pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
    pkg_check_modules(gio REQUIRED IMPORTED_TARGET gio-2.0)
    pkg_check_modules(gmodule REQUIRED IMPORTED_TARGET gmodule-2.0)
    pkg_check_modules(gobject REQUIRED IMPORTED_TARGET gobject-2.0)
    pkg_check_modules(gthread REQUIRED IMPORTED_TARGET gthread-2.0)
    pkg_check_modules(libsecret REQUIRED IMPORTED_TARGET libsecret-1)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads PkgConfig::sqlcipher PkgConfig::glib PkgConfig::gio PkgConfig::gmodule PkgConfig::gobject PkgConfig::gthread PkgConfig::libsecret)
endif()

#libnick Install
configure_file("${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}.pc" @ONLY)
configure_package_config_file("${CMAKE_SOURCE_DIR}/cmake/config.cmake.in" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
write_basic_package_version_file("${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake" VERSION "${PROJECT_VERSION}" COMPATIBILITY AnyNewerVersion)
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}Targets" LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(FILES "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
install(EXPORT "${PROJECT_NAME}Targets" FILE "${PROJECT_NAME}Targets.cmake" NAMESPACE ${PROJECT_NAME}:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(FILES "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake" "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
export(EXPORT "${PROJECT_NAME}Targets" FILE "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake" NAMESPACE ${PROJECT_NAME}::)

#libnick Test
if (BUILD_TESTING)
    add_executable(${PROJECT_NAME}_test
    "tests/codetests.cpp"
    "tests/databasetests.cpp"
    "tests/eventtests.cpp"
    "tests/filewatchertests.cpp"
    "tests/hardwaretests.cpp"
    "tests/jsonfiletests.cpp"
    "tests/keyringtests.cpp"
    "tests/localizationtests.cpp"
    "tests/main.cpp"
    "tests/networktests.cpp"
    "tests/notificationtests.cpp"
    "tests/passwordtests.cpp"
    "tests/processtests.cpp"
    "tests/stringtests.cpp"
    "tests/systemtests.cpp"
    "tests/updatertests.cpp"
    "tests/versiontests.cpp"
    "tests/webtests.cpp")
    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main ${PROJECT_NAME})
endif()
