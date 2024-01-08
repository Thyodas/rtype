cmake_minimum_required(VERSION 3.17)

project(game_engine)

set(SRCS
    src/game_engine/Editor.cpp
    src/game_engine/editor/GameEngineEditor.cpp
    src/game_engine/editor/DocumentWindows/Main3DScene.cpp
    src/game_engine/editor/DocumentWindows/SceneTreeWindow.cpp
    src/game_engine/editor/DocumentWindows/EntityPropertiesWindow.cpp
    src/game_engine/core/GameEngine.cpp
    src/game_engine/core/window/Window.cpp
    src/game_engine/core/event/Event.cpp
    src/game_engine/ecs/components/Shapes.cpp
    src/game_engine/ecs/components/Behaviour.cpp
    src/game_engine/ecs/systems/Render.cpp
    src/game_engine/ecs/systems/Physics.cpp
    src/game_engine/ecs/systems/Behaviour.cpp
    src/game_engine/ecs/systems/Animations.cpp
    src/game_engine/ecs/systems/Input.cpp
    src/game_engine/ecs/Entity.cpp

)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch and include rlImGui
include(FetchContent)
FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rlimgui)

add_executable(game_engine ${SRCS})

# Setup include directories
include_directories(include)
include_directories("./include/game_engine")
include_directories("./include/common")

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

# Apple-specific settings
if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra --std=c++20 -O3 -g")
endif()

# Find Boost package
find_package(Boost REQUIRED COMPONENTS serialization system)
include_directories(${Boost_INCLUDE_DIRS})

# Find raylib package
find_package(raylib 5.0 CONFIG QUIET)
include_directories(${raylib_INCLUDE_DIRS})

# Find imgui package
find_package(imgui QUIET)
include_directories(${imgui_INCLUDE_DIRS})


# Linking the libraries
target_link_libraries(game_engine PRIVATE raylib imgui::imgui rlimgui)

# Compiler options for Unix
if(UNIX)
    set(CMAKE_CXX_FLAGS "--std=c++20 -Wall -Wextra -O3 -g")
endif()

include_directories(include)
include_directories("./include/game_engine")
include_directories("./include/common")

add_dependencies(game_engine package)
