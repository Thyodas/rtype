cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

# PROJECT
project(client CXX)

# COMPILATION OPTIONS
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-Wall -std=c++2a)

# COMPILING RAYLIB
# execute_process(COMMAND ${CMAKE_SOURCE_DIR}/raylib_build.sh)

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
if(UNIX)
        include(FetchContent)
        FetchContent_Declare(
                raylib
                DOWNLOAD_EXTRACT_TIMESTAMP OFF
                URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
        )
        FetchContent_MakeAvailable(raylib)
endif(UNIX)

# EXECUTABLES
# if(WIN32)
#         add_executable(client WIN32 ${CLIENT_SOURCE})
# else()
#         add_executable(client ${CLIENT_SOURCE})
# endif(WIN32)

# if(WIN32)
#         add_executable(server WIN32 ${SERVER_SOURCE})
# else()
#         add_executable(server ${SERVER_SOURCE})
# endif()


add_executable(client ${CLIENT_SOURCE})
add_executable(server ${SERVER_SOURCE})
install(TARGETS client RUNTIME DESTINATION bin CONFIGURATIONS Release)
install(TARGETS server RUNTIME DESTINATION bin CONFIGURATIONS Release)

# target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

if(WIN32)
        set(raylib_DIR ".\\vcpkg\\packages\\raylib_x64-windows\\share\\raylib")
endif(WIN32)

find_package(raylib 5.0 CONFIG REQUIRED)
target_link_libraries(client PRIVATE raylib)
# target_include_directories(client PUBLIC "./src")

# CPACK CONFIGURATION
configure_file ("${PROJECT_SOURCE_DIR}/RTypeCPackConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/CPackSourceConfig.cmake"
        @ONLY)
set (CPACK_PROJECT_CONFIG_FILE
        "${PROJECT_BINARY_DIR}/RTypeCPackConfig.cmake")

# INCLUDES
include(CPack)
