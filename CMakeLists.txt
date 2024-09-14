cmake_minimum_required(VERSION 3.16)

project(TinyGraphics VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options("/utf-8")
	add_compile_options(/Zc:preprocessor)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# package
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdParty/glfw)
find_package(OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdParty)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui)
# !end

set(
    SOURCE_IMGUI
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/backends/imgui_impl_glfw.cpp
)
set(
    SOURCE_TG
    ${CMAKE_CURRENT_LIST_DIR}/tg/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tg/ui/window.cpp
)
file(GLOB SOURCE_EXAMPLE example/*.cpp)

add_executable(
	TinyGraphics
    ${SOURCE_GLFW}
    ${SOURCE_IMGUI}
    ${SOURCE_TG}
	${SOURCE_EXAMPLE}
)
target_link_libraries(
    TinyGraphics
    PRIVATE
    glfw
    OpenGL::GL
)
