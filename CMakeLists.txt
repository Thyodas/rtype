cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

# PROJECT
project(client CXX)

# COMPILATION OPTIONS
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-Wall -std=c++2a)

if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
endif()

# SOURCES
set(CLIENT_SRC
        main.cpp
)

set(COMMON_SRC
)

set(SERVER_SRC
        main.cpp
)

#SOURCES TRANSFORMATIONS
list(TRANSFORM CLIENT_SRC PREPEND "client/")
list(TRANSFORM COMMON_SRC PREPEND "common/")
list(TRANSFORM SERVER_SRC PREPEND "server/")


list(APPEND CLIENT_SOURCE ${CLIENT_SRC} ${COMMON_SRC})
list(TRANSFORM CLIENT_SOURCE PREPEND "src/")

list(APPEND SERVER_SOURCE ${COMMON_SRC} ${SERVER_SRC})
list(TRANSFORM SERVER_SOURCE PREPEND "src/")

# LIBRARIES
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



add_executable(client ${CLIENT_SOURCE})
add_executable(server ${SERVER_SOURCE})
install(TARGETS client RUNTIME DESTINATION bin CONFIGURATIONS Release)
install(TARGETS server RUNTIME DESTINATION bin CONFIGURATIONS Release)


if(WIN32)
    include_directories(${raylib_INCLUDE_DIR})
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\vcpkg_installed\\x64-windows\\include")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}\\vcpkg_installed\\x64-windows\\bin")
endif(WIN32)

find_package(raylib 5.0 CONFIG QUIET)

target_link_libraries(client PRIVATE raylib)
if(WIN32)
        target_link_libraries(client INTERFACE raylib)
endif()
# CPACK CONFIGURATION
configure_file ("${PROJECT_SOURCE_DIR}/RTypeCPackConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/CPackSourceConfig.cmake"
        @ONLY)
set (CPACK_PROJECT_CONFIG_FILE
        "${PROJECT_BINARY_DIR}/RTypeCPackConfig.cmake")

# INCLUDES
include(CPack)
