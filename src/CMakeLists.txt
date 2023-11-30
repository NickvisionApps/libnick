add_library (libaura SHARED
events/eventargs.cpp
events/notificationsenteventargs.cpp
events/shellnotificationsenteventargs.cpp
helpers/stringhelpers.cpp
helpers/webhelpers.cpp
keyring/credential.cpp
keyring/passwordgenerator.cpp
update/updater.cpp
appinfo.cpp
aura.cpp
configurationbase.cpp
dependencylocator.cpp
systemdirectories.cpp
userdirectories.cpp
version.cpp)

set_target_properties(libaura PROPERTIES 
VERSION ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

#libintl
find_package(Intl REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Intl::Intl)
#maddy
find_package(unofficial-maddy CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::maddy::maddy)
#jsoncpp
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC JsonCpp::JsonCpp)
#cURLpp
find_package(unofficial-curlpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC unofficial::curlpp::curlpp)
#sqlitecpp with sqlcipher
find_package(sqlcipher CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC sqlcipher::sqlcipher)
find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SQLiteCpp)
#dbus
if(LINUX)
	find_package(DBus1 CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME} PUBLIC dbus-1)
endif()