cmake_minimum_required(VERSION 3.17)

project(sampleJolt)

set(SRCS
    joltSample.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(sampleJolt ${SRCS} ${HEADERS})

if(WIN32)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include")
elseif(UNIX AND NOT APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-subbuild")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-build/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-linux/include")
elseif(APPLE)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-subbuild")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-build/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-osx/include")
endif()

find_package(Boost REQUIRED COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIRS})

if(WIN32)
    find_package(raylib CONFIG QUIET)
else()
	find_package(raylib 5.0 CONFIG QUIET)
endif(WIN32)

include_directories(${raylib_INCLUDE_DIRS})

find_package(unofficial-joltphysics CONFIG REQUIRED)
include_directories(${unofficial-joltphysics_INCLUDE_DIRS})

if(WIN32)
    target_link_libraries(sampleJolt PRIVATE raylib unofficial::joltphysics::Jolt Boost::serialization Boost::system)
else()
    target_link_libraries(sampleJolt PRIVATE raylib unofficial::joltphysics::Jolt Boost::serialization)
endif(WIN32)

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)
include_directories("./include/sampleJolt")
include_directories("./include/common")
