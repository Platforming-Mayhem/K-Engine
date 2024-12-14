cmake_minimum_required(VERSION 3.5)

project(K-Engine)

set(CMAKE_CXX_STANDARD 23)
set(USE_GLU ON)
set(ONLY_LIBS ON)
set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)

find_package(OpenGL REQUIRED)
add_subdirectory(Vendors/GLFW)
add_subdirectory(Vendors/GLEW)
add_subdirectory(Vendors/IMGUI)

add_compile_definitions(K_BUILD_DLL)

add_library(${PROJECT_NAME} SHARED 
Vendors/IMGUI/backends/imgui_impl_glfw.cpp
Vendors/IMGUI/backends/imgui_impl_opengl3.cpp
src/K-Engine/Application.cpp
src/K-Engine/Editor.cpp
src/K-Engine/GameObject.cpp
src/K-Engine/InputManager.cpp
src/K-Engine/Material.cpp
src/K-Engine/RenderTexture.cpp
src/K-Engine/SceneManager.cpp
src/K-Engine/Serializer.cpp
src/K-Engine/Shader.cpp
src/K-Engine/Texture.cpp
src/K-Engine/Time.cpp
src/K-Engine/Transform.cpp
src/K-Engine/Window.cpp
src/K-Engine/Components/Component.cpp
)

target_link_libraries(${PROJECT_NAME} OpenGL::GLU)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} IMGUI)
target_link_libraries(${PROJECT_NAME} libglew_static)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/assets)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendors/STB)