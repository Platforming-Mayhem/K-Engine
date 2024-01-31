#include "SceneManager.h"
#include "Editor.h"

namespace K
{
	K::Scene* K::SceneManager::currentScene;

	SceneManager::SceneManager(unsigned int resource)
	{
		HMODULE hModule;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "main", &hModule);
		HRSRC hr = FindResource(hModule, MAKEINTRESOURCE(resource), "SCENES");
		if (hr != NULL) 
		{
			int size = SizeofResource(hModule, hr);
			HGLOBAL temp = LoadResource(hModule, hr);
			if (temp) 
			{
				LPVOID lp = LockResource(temp);
				std::stringstream inFile;
				const char* data = (const char*)lp;
				inFile << data;
				std::string line;
				while (std::getline(inFile, line))
				{
					line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
					this->AddScene(line.c_str());
				}

				UnlockResource(temp);
				inFile.clear();
			}
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
		K::Editor::selectedGameObject = nullptr;
		if (this->scenes.size() > 0) 
		{
			std::string location = this->scenes[index];
			std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
			K::Deserializer deserializer = K::Deserializer(K::SceneManager::currentScene, this->scenes[index]);
		}
		else 
		{
			K::SceneManager::currentScene->Init();
		}
	}

	void SceneManager::DeleteScene(int index) 
	{
		this->scenes.erase(this->scenes.begin() + index);
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
				outFile << '\n';
			}
		}
		outFile.close();
	}
}