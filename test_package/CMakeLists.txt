cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(libaura CONFIG REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example libaura::libaura JsonCpp::JsonCpp)
