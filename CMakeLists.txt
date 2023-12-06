cmake_minimum_required (VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SQLITE_HAS_CODEC ON)
if(WIN32)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(BUILD_SHARED_LIBS ON)
endif()
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("libaura" LANGUAGES CXX VERSION 2023.12.0 DESCRIPTION "A cross-platform base for native Nickvision applications.")
include(GNUInstallDirs)

#libaura Library Setup
include_directories(${PROJECT_SOURCE_DIR}/include)
add_library (${PROJECT_NAME} SHARED
	src/events/eventargs.cpp
	src/helpers/stringhelpers.cpp
	src/helpers/webhelpers.cpp
	src/keyring/credential.cpp
	src/keyring/keyring.cpp
	src/keyring/keyringdialogcontroller.cpp
	src/keyring/passwordgenerator.cpp
	src/keyring/store.cpp
	src/keyring/systemcredentials.cpp
	src/network/networkmonitor.cpp
	src/network/networkstatechangedeventargs.cpp
	src/notifications/notificationsenteventargs.cpp
	src/notifications/shellnotificationsenteventargs.cpp
	src/update/updater.cpp
	src/appinfo.cpp
	src/aura.cpp
	src/configurationbase.cpp
	src/dependencylocator.cpp
	src/systemdirectories.cpp
	src/userdirectories.cpp
	src/version.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION})

#libaura Packages
find_package(Intl REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(CURL REQUIRED)
find_package(maddy REQUIRED)
find_package(SQLiteCpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Intl::Intl JsonCpp::JsonCpp CURL::libcurl maddy::maddy SQLiteCpp)
if(LINUX)
	find_package(PkgConfig REQUIRED)
	find_package(libsecret REQUIRED CONFIG)
	pkg_check_modules(glib-2.0 REQUIRED IMPORTED_TARGET glib-2.0)
	pkg_check_modules(gio-2.0 REQUIRED IMPORTED_TARGET gio-2.0)
	pkg_check_modules(gmodule-2.0 REQUIRED IMPORTED_TARGET gmodule-2.0)
	pkg_check_modules(gobject-2.0 REQUIRED IMPORTED_TARGET gobject-2.0)
	pkg_check_modules(gthread-2.0 REQUIRED IMPORTED_TARGET gthread-2.0)
	pkg_search_module(UUID REQUIRED uuid)
	target_include_directories(${PROJECT_NAME} PUBLIC ${UUID_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PUBLIC resolv libsecret::libsecret PkgConfig::glib-2.0 PkgConfig::gio-2.0 PkgConfig::gmodule-2.0 PkgConfig::gobject-2.0 PkgConfig::gthread-2.0 ${UUID_LIBRARIES})
endif()

#libaura Install
configure_file(${PROJECT_SOURCE_DIR}/pkg-config/libaura.pc.in libaura.pc @ONLY)
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/libaura.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#libaura Tests
enable_testing()

add_executable(${PROJECT_NAME}_test
	tests/eventtests.cpp
	tests/main.cpp
	tests/networktests.cpp
	tests/stringtests.cpp
	tests/versiontests.cpp
	tests/webtests.cpp)

find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME}_test PUBLIC libaura GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)