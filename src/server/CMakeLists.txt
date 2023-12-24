cmake_minimum_required(VERSION 3.17)

project(server)

set(SRCS
    Main.cpp
    core/Server.cpp
    core/NetServer.cpp
    ../common/game/entities/Entity.cpp
    ../common/game/entities/EntityFactory.cpp
    ../common/network/Server.cpp
    ../game_engine/core/GameEngine.cpp
    ../game_engine/core/window/Window.cpp
    ../game_engine/core/event/Event.cpp
    ../game_engine/ecs/components/Shapes.cpp
    ../game_engine/ecs/components/Behaviour.cpp
    ../game_engine/ecs/systems/Render.cpp
    ../game_engine/ecs/systems/Physics.cpp
    ../game_engine/ecs/systems/Behaviour.cpp
    ../game_engine/ecs/systems/Animations.cpp
    ../game_engine/ecs/Entity.cpp
)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIRS})

find_package(raylib 5.0 QUIET)
include_directories(${raylib_INCLUDE_DIRS})

if (APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
else()
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)
include_directories(../../include)

add_executable(server ${SRCS} ${HEADERS})

target_link_libraries(server ${Boost_LIBRARIES} raylib Boost::serialization)
