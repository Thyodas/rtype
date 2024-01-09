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

set_target_properties( game_engine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)


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


# Copying exe to build directory
if(WIN32)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/client.exe" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 1/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/client.exp" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 2/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/client.lib" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 3/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/server.exe" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 4/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/server.exp" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 5/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/server.lib" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 6/10\n"
    VERBATIM
)

add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/boost_serialization-vc143-mt-x64-1_83.dll" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 7/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/game_engine.exe" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 8/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/raylib.dll" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 9/10\n"
    VERBATIM
)
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/rlimgui.lib" "${CMAKE_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying exe to build directory 10/10\n"
    VERBATIM
)
endif(WIN32)

# Copying resouces to build directory
# add_custom_command(
#     TARGET game_engine POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/ressources" "${CMAKE_BINARY_DIR}/"
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMENT "Copying resouces to build directory"
#     VERBATIM
# )

# Creating archive after all targets are built
add_custom_command(
    TARGET game_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar "cf" "package.tar" "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/ressources"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Creating package.tar"
    VERBATIM
)
