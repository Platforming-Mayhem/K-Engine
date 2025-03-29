#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <strstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <numbers>
#include <unordered_map>
#include <format>
#include <thread>
#include <ignore/resource.h>
#include <bitset>

//GLFW & GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//NFD
#include <nfd.h>

//OpenAL
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

//ImGUI
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#ifdef _WIN32
#include <Windows.h>
#elif __unix__
#include <dlfcn.h>
#endif

#ifdef K_BUILD_DLL
	#define K_API __declspec(dllexport)
#else
	#define K_API __declspec(dllimport)
#endif

#ifdef KC_BUILD_DLL
#define KC_API __declspec(dllexport)
#else
#define KC_API __declspec(dllimport)
#endif

K_API extern std::string ASSET_DIR;