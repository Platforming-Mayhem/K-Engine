cmake_minimum_required(VERSION 3.8)

project(K-Engine)

IF(UNIX)
add_compile_options("-fPIC")
ADD_DEFINITIONS(-D_DEBUG)
ENDIF()
set(CMAKE_CXX_STANDARD 23)
set(USE_GLU ON)

find_package(OpenGL REQUIRED)

add_subdirectory(vendors/GLFW)
add_subdirectory(vendors/IMGUI)
add_subdirectory(vendors/GLEW)
add_subdirectory(vendors/STB)
add_subdirectory(vendors/MiniAudio)
add_subdirectory(vendors/NativeFileDialog-Extended)

add_library(${PROJECT_NAME} SHARED 
src/K-Engine/Application.cpp
src/K-Engine/Editor.cpp
src/K-Engine/GameObject.cpp
src/K-Engine/InputManager.cpp
src/K-Engine/Material.cpp
src/K-Engine/RenderTexture.cpp
src/K-Engine/Scene.cpp
src/K-Engine/SceneManager.cpp
src/K-Engine/Serializer.cpp
src/K-Engine/Shader.cpp
src/K-Engine/Texture.cpp
src/K-Engine/Audio.cpp
src/K-Engine/Time.cpp
src/K-Engine/Transform.cpp
src/K-Engine/Window.cpp
src/K-Engine/Components/Component.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE K_BUILD_DLL)

target_link_libraries(${PROJECT_NAME} OpenGL::GLU)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} IMGUI)
target_link_libraries(${PROJECT_NAME} stb)
target_link_libraries(${PROJECT_NAME} nfd)
target_link_libraries(${PROJECT_NAME} libglew_shared)
target_link_libraries(${PROJECT_NAME} miniaudio)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/assets)

project(Editor)

IF (MSVC)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ENDIF()

add_executable(${PROJECT_NAME}
src/Editor/Editor.cpp
)

IF (MSVC)
add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> 
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)
ENDIF()

target_link_libraries(${PROJECT_NAME} K-Engine)

IF (MSVC)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)/$(Configuration)")
ENDIF()
