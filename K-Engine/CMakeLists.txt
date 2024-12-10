cmake_minimum_required(VERSION 3.12)

project(K-Engine)

file(GLOB_RECURSE SOURCES *.c**)
file(GLOB_RECURSE HEADERS *.h**)
target_compile_definitions(_CRT_SECURE_NO_WARNINGS)
target_compile_definitions(GLEW_STATIC)
target_compile_definitions(K_BUILD_DLL)
add_library(K-Engine SHARED ${SOURCES} ${HEADERS})