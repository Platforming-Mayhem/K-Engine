#include "SceneManager.h"

namespace K
{
	K::Scene* K::SceneManager::currentScene;

	SceneManager::SceneManager(unsigned int resource)
	{
		HMODULE hModule;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "main", &hModule);
		HRSRC hr = FindResource(hModule, MAKEINTRESOURCE(resource), "SCENES");
		int size = SizeofResource(hModule, hr);
		if (hr != NULL) 
		{
			HGLOBAL temp = LoadResource(hModule, hr);
			LPVOID lp = LockResource(temp);
			std::stringstream inFile;
			const char* data = (const char*)lp;
			inFile << data;
			std::string line;
			while (std::getline(inFile, line)) 
			{
				std::cout << "Found scene: " << line.c_str() << std::endl;
				this->AddScene(line);
			}
			UnlockResource(temp);
			inFile.clear();
		}
		else 
		{
			std::cout << "No scenes were found in the build" << std::endl;
		}
		K::SceneManager::currentScene = new K::Scene("Untitled*");
	}

	SceneManager::~SceneManager() 
	{
		delete this->currentScene;
		this->scenes.clear();
	}

	int SceneManager::GetNumberOfScenes() 
	{
		return this->scenes.size();
	}

	const char* SceneManager::GetSceneName(int index) 
	{
		return this->scenes[index].c_str();
	}

	void SceneManager::LoadScene(int index) 
	{
		K::SceneManager::currentScene->CreateEmptyScene();
		K::Deserializer deserializer = K::Deserializer(K::SceneManager::currentScene, this->scenes[index]);
	}

	void SceneManager::AddScene(std::string name)
	{
		this->scenes.push_back(name);
	}

	void SceneManager::SaveSceneManager() 
	{
		std::ofstream outFile;
		outFile.open(ASSET_DIR + "SceneManager.txt", std::ofstream::trunc);
		if (outFile)
		{
			for (int i = 0; i < this->scenes.size(); i++) 
			{
				outFile << this->scenes[i];
				if(i < this->scenes.size())
					outFile << '\n';
			}
		}
		outFile.close();
	}
}