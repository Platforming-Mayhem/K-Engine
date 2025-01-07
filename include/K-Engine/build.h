#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
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
#include <Windows.h>
#include <ignore/resource.h>
#include <bitset>

//ImGUI
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef K_BUILD_DLL
	#define K_API __declspec(dllexport)
#else
	#define K_API __declspec(dllimport)
#endif

K_API extern std::string ASSET_DIR;