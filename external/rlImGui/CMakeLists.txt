cmake_minimum_required(VERSION 3.17)
project(rlImGui)

# Find raylib and imgui packages
find_package(raylib QUIET)
find_package(imgui REQUIRED)

# Fetch rlImGui using FetchContent
include(FetchContent)
FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rlimgui)

# List of rlImGui source files
set(RLIMGUI_SOURCES
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)

# Create rlImGui library
add_library(rlimgui STATIC ${RLIMGUI_SOURCES})

# Link dependencies (raylib, ImGui)
target_link_libraries(rlimgui PRIVATE raylib imgui::imgui)

# Include directories for rlImGui
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
