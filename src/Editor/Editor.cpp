#include <K_Engine.h>
#ifdef _WIN32
	#include <Windows.h>
#elif __unix__
	#include <dlfcn.h>
#endif

void main(int argc, char** argv)
{
	K::Application* app = new K::Application();

	NFD_Init();
	char* location;
	if (NFD_PickFolderU8(&location, NULL) == NFD_OKAY)
	{
		ASSET_DIR = location;
		ASSET_DIR += '/';
		K::Editor::SetDirectory(ASSET_DIR);
		NFD_FreePathU8(location);
	}
	NFD_Quit();

	std::string cmakelist = ASSET_DIR + "CMakeLists.txt";

	FILE* file = fopen(cmakelist.c_str(), "w");

	fputs("set(CMAKE_CXX_STANDARD 23) \n"
		"set(USE_GLU ON) \n"
		"set(ONLY_LIBS ON) \n"
		"project(Components) \n", file);

	std::string cmakeCommand = std::format("set(CMAKE_LIBRARY_OUTPUT_DIRECTORY {0}) \n", std::filesystem::current_path().parent_path().string());

	fputs(cmakeCommand.c_str(), file);

	cmakeCommand = std::format("set(CMAKE_RUNTIME_OUTPUT_DIRECTORY {0}) \n", std::filesystem::current_path().parent_path().string());

	fputs(cmakeCommand.c_str(), file);

	cmakeCommand = std::format("add_subdirectory(\"{0}\" K-Engine) \n", std::filesystem::current_path().parent_path().parent_path().string());

	fputs(cmakeCommand.c_str(), file);

	fputs("file(GLOB ComponentSrcs \"components/*.cpp\") \n"
		"add_library(${PROJECT_NAME} SHARED ${ComponentSrcs}) \n"
		"target_compile_definitions(${PROJECT_NAME} PRIVATE KC_BUILD_DLL) \n"
		"target_link_libraries(${PROJECT_NAME} K-Engine) \n"
		"set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY \"$(ProjectDir) / $(Configuration)\")", file);

	fclose(file);

	const std::string quote = "\"";

	cmakelist = "cmake -S " + quote + ASSET_DIR + quote + " -B " + quote + ASSET_DIR + "bin" + quote;

	std::system(cmakelist.c_str());

	#ifdef _WIN32
	HINSTANCE hInst = LoadLibrary(TEXT("Components"));

	if (hInst != NULL)
	{
		std::cout << "Found Library" << std::endl;
	}
	else 
	{
		std::cout << "No Library Found" << std::endl;
	}

	#elif __unix__
	void* handle = dlopen("Components.so", RTLD_NOW)
	if (handle) 
	{
		std::cout << "Found Library" << std::endl;
	}
	#endif

	std::string name = argv[0];
	app->Run(name.substr(name.find_last_of('\\') + 1).c_str());

	#ifdef _WIN32
	FreeLibrary(hInst);
	#elif __unix__
	dlclose(handle);
	#endif
	delete app;
}