cmake_minimum_required(VERSION 3.8)

project(K-Engine)

set(CMAKE_CXX_STANDARD 23)
set(USE_GLU ON)

find_package(OpenGL REQUIRED)

add_subdirectory(Vendors/GLFW)
add_subdirectory(Vendors/IMGUI)
add_subdirectory(Vendors/GLEW)
add_subdirectory(Vendors/MiniAudio)
add_subdirectory(Vendors/NativeFileDialog-Extended)

add_library(${PROJECT_NAME} SHARED 
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

target_sources(${PROJECT_NAME} PRIVATE assets/ignore/KEngineResources.rc)

target_compile_definitions(${PROJECT_NAME} PRIVATE K_BUILD_DLL)

target_link_libraries(${PROJECT_NAME} OpenGL::GLU)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} IMGUI)
target_link_libraries(${PROJECT_NAME} nfd)
target_link_libraries(${PROJECT_NAME} libglew_static)
target_link_libraries(${PROJECT_NAME} miniaudio)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/assets)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendors)

project(Editor)

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

add_executable(${PROJECT_NAME}
src/Editor/Editor.cpp
)

target_sources(${PROJECT_NAME} PRIVATE assets/ignore/Editor.rc)

target_link_libraries(${PROJECT_NAME} K-Engine)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)/$(Configuration)")