cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

# PROJECT
project(client)

# COMPILATION OPTIONS
add_compile_options(-Wall -Wextra -std=c++2a)

# INCLUDES
include(CPack)

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

# EXECUTABLES
add_executable(client ${CLIENT_SOURCE})
install(TARGETS client RUNTIME DESTINATION bin)

add_executable(server ${SERVER_SOURCE})
install(TARGETS server RUNTIME DESTINATION bin)


# CPACK CONFIGURATION
configure_file ("${PROJECT_SOURCE_DIR}/RTypeCPackConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/CPackSourceConfig.cmake"
        @ONLY LF)
set (CPACK_PROJECT_CONFIG_FILE
        "${PROJECT_BINARY_DIR}/RTypeCPackConfig.cmake")
