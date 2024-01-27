#pragma once
#include <iostream>
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <filesystem>
#include <fstream>
#include <strstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <format>
#include "resource.h"

//ImGUI
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imfilebrowser.h" 

#ifdef K_BUILD_DLL
	#define K_API __declspec(dllexport)
#else
	#define K_API __declspec(dllimport)
#endif