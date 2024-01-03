cmake_minimum_required(VERSION 3.17)

include(FetchContent)
FetchContent_Declare(
        imgui
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY https://github.com/adobe/imgui.git
        GIT_TAG        f797da726edecd563ad3467e22095e8ff89ba538
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    set(IMGUI_USE_DARK_THEME ON)
    FetchContent_Populate(imgui)
    add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
endif()
