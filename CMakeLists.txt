cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

# PROJECT
project(client CXX)

# RUNNING VCPKG
if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
else()
    include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# RAYLIB DOWNLOAD FOR UNIX
if (UNIX)
    find_package(raylib 5.0 QUIET)
    if (NOT raylib_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                raylib
                DOWNLOAD_EXTRACT_TIMESTAMP OFF
                URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
        )
        FetchContent_GetProperties(raylib)
        if (NOT raylib_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_Populate(raylib)
            set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
            add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        endif()
    endif()
endif()


include("${CMAKE_CURRENT_SOURCE_DIR}/src/client/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/server/CMakeLists.txt")
