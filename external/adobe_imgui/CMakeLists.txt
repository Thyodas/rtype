cmake_minimum_required(VERSION 3.17)

include(FetchContent)
if (MACOS)
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/adobe/imgui.git
            GIT_BRANCH     docking_v1.90
            GIT_TAG        49d3ce20dfecc25919dfd6ca4cb4b10cd6e30690
    )
else()
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/adobe/imgui.git
            GIT_TAG        49d3ce20dfecc25919dfd6ca4cb4b10cd6e30690
    )
endif()
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    set(IMGUI_USE_DARK_THEME ON)
    FetchContent_Populate(imgui)
    add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
endif()
