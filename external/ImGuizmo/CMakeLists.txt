cmake_minimum_required(VERSION 3.17)
project(YourProjectName)

# Include FetchContent module
include(FetchContent)

# Declare ImGuizmo dependency
FetchContent_Declare(
        imguizmo
        URL https://github.com/CedricGuillemet/ImGuizmo/archive/refs/tags/1.83.tar.gz
        URL_HASH SHA512=23285398688b4cdf3128ecb918b70c9a52f06c8e911da57430442b838cecf653e233d8cdfefc6acd3e4da381869ffc6fb74bcaaafc8e33657d6060a645517363
)

# Fetch the content at configure time
FetchContent_MakeAvailable(imguizmo)

# Find imgui package
find_package(imgui QUIET)
include_directories(${imgui_INCLUDE_DIRS})

# Define ImGuizmo library
add_library(imguizmo STATIC
            ${imguizmo_SOURCE_DIR}/GraphEditor.cpp
            ${imguizmo_SOURCE_DIR}/ImCurveEdit.cpp
            ${imguizmo_SOURCE_DIR}/ImGradient.cpp
            ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
            ${imguizmo_SOURCE_DIR}/ImSequencer.cpp
)

target_compile_options(imguizmo PRIVATE "-std=c++11")

# Include directories for ImGuizmo
target_include_directories(imguizmo PUBLIC
                           $<BUILD_INTERFACE:${imguizmo_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:include>
                           PRIVATE ${IMGUI_INCLUDE_DIRS}
)

# Link ImGui to ImGuizmo
target_link_libraries(imguizmo PRIVATE imgui::imgui)
