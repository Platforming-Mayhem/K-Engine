cmake_minimum_required(VERSION 3.5)

project(IMGUI)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} STATIC
imgui.cpp
imgui_demo.cpp
imgui_draw.cpp
imgui_tables.cpp
imgui_widgets.cpp
misc/cpp/imgui_stdlib.cpp
backends/imgui_impl_glfw.cpp
backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC backends misc/cpp ${PROJECT_SOURCE_DIR} examples/libs/glfw/include)
