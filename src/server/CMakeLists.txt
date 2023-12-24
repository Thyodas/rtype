cmake_minimum_required(VERSION 3.17)

project(server)

set(SRCS
    src/server/Main.cpp
    src/server/core/Server.cpp
    src/server/core/NetServer.cpp
    src/common/game/entities/Entity.cpp
    src/common/game/entities/EntityFactory.cpp
    src/common/network/Server.cpp
    src/game_engine/core/GameEngine.cpp
    src/game_engine/core/window/Window.cpp
    src/game_engine/core/event/Event.cpp
    src/game_engine/ecs/components/Shapes.cpp
    src/game_engine/ecs/components/Behaviour.cpp
    src/game_engine/ecs/systems/Render.cpp
    src/game_engine/ecs/systems/Physics.cpp
    src/game_engine/ecs/systems/Behaviour.cpp
    src/game_engine/ecs/systems/Animations.cpp
    src/game_engine/ecs/Entity.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server ${SRCS} ${HEADERS})

if(WIN32)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/raylib/Debug")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/bin")
else()
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-subbuild")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-build/raylib/include")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-linux/include")
endif()

find_package(Boost REQUIRED COMPONENTS serialization system)
include_directories(${boost_INCLUDE_DIRS})

if(WIN32)
    find_package(raylib CONFIG QUIET)
else()
	find_package(raylib 5.0 CONFIG QUIET)
endif(WIN32)
include_directories(${raylib_INCLUDE_DIRS})

if(WIN32)
    target_link_libraries(server PRIVATE raylib Boost::serialization Boost::system)
else()
    target_link_libraries(server PRIVATE raylib)
    target_link_libraries(server PRIVATE Boost::serialization)
endif(WIN32)


if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)
include_directories("./include/common")
include_directories("./include/server")
