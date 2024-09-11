cmake_minimum_required(VERSION 3.16)

project(TinyGraphics VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options("/utf-8")
	add_compile_options(/Zc:preprocessor)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_SOURCES "ui/*.cpp" "private/*.cpp")
set(PROJECT_SOURCES tg/main.cpp tg/utils.cpp ${SRC_SOURCES})

# package
set(OpenCV_DIR "C:/home/bin/opencv/build")
find_package(OpenCV REQUIRED)
include_directories("C:/home/bin/opencv/build/include")
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories("C:/home/bin")
# !end

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(
	TinyGraphics
	${PROJECT_SOURCES}
)

target_link_libraries(TinyGraphics PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS}
)
set_target_properties(TinyGraphics PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE FALSE
)
