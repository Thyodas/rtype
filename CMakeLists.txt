cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# PROJECT
project(client CXX)


# SETUP GIT SUBMODULES

find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# SETUP VCPKG

if (WIN32)
    execute_process(
            COMMAND .\\vcpkg\\bootstrap-vcpkg.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    execute_process(
            COMMAND ./vcpkg/bootstrap-vcpkg.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

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

# RLIMGUI DOWNLOAD
include("${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui/CMakeLists.txt")

# SETUP CLIENT, SERVER AND GAME ENGINE GUI
include("${CMAKE_CURRENT_SOURCE_DIR}/src/client/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/server/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/src/game_engine/CMakeLists.txt")
