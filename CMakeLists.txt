cmake_minimum_required(VERSION 3.8)

file(GLOB ComponentSrcs
     "assets/components/*.cpp"
)

project(K-Engine)

set(CMAKE_CXX_STANDARD 23)
set(USE_GLU ON)
set(ONLY_LIBS ON)

find_package(OpenGL REQUIRED)

add_subdirectory(Vendors/GLFW)
add_subdirectory(Vendors/IMGUI)
add_subdirectory(Vendors/GLEW)
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
target_link_libraries(${PROJECT_NAME} libglew_shared)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/assets)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendors/STB)

project(Components)

add_library(${PROJECT_NAME} SHARED ${ComponentSrcs})

target_compile_definitions(${PROJECT_NAME} PRIVATE KC_BUILD_DLL)

target_link_libraries(${PROJECT_NAME} K-Engine)

project(Editor)

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

add_executable(${PROJECT_NAME}
src/Editor/Editor.cpp
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> 
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)

target_link_libraries(${PROJECT_NAME} K-Engine)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)/$(Configuration)")