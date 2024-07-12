#include "SceneManager.h"
#include "Editor.h"

namespace K
{
	std::vector<std::string> K::SceneManager::scenes;
	K::Scene* K::SceneManager::currentScene;
	bool K::SceneManager::loadNextScene = false;
	bool K::SceneManager::loadScene = false;
	int K::SceneManager::index = -1;

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
		return K::SceneManager::scenes[index].c_str();
	}

	void SceneManager::LoadScene(int newIndex) 
	{
		K::SceneManager::loadScene = true;
		K::SceneManager::index = newIndex;
		K::SceneManager::loadNextScene = false;
	}

	bool SceneManager::IsLoadingNextScene() 
	{
		return K::SceneManager::loadNextScene;
	}

	void SceneManager::LoadNextScene() 
	{
		K::SceneManager::loadNextScene = true;
	}

	void SceneManager::Update() 
	{
		if (!K::Editor::deleteArray.empty())
		{
			for (K::GameObject* pointer : K::Editor::deleteArray)
			{
				if (pointer->parent != nullptr) 
				{
					pointer->SetParent(nullptr);
				}
				if (!pointer->children.empty()) 
				{
					for (int i : pointer->children) 
					{
						K::Editor::GetCurrentScene()->GetGameObjects().at(i)->SetParent(nullptr);
					}
				}
				K::Editor::GetCurrentScene()->Delete(pointer);
			}
			K::Editor::deleteArray.clear();
		}
		if (K::SceneManager::loadNextScene) 
		{
			int index = -1;
			std::string currentScene = K::SceneManager::currentScene->GetSceneName();
			for (int i = 0; i < K::SceneManager::scenes.size(); i++)
			{
				if (currentScene.find(scenes[i]) != std::string::npos)
				{
					index = i;
					break;
				}
			}
			if (index > -1)
			{
				index++;
				K::SceneManager::LoadScene(index);
			}
		}
		if (K::SceneManager::loadScene) 
		{
			K::Editor::selectedGameObject = nullptr;
			K::SceneManager::currentScene->CreateEmptyScene();
			if (K::SceneManager::scenes.size() > 0)
			{
				if (K::SceneManager::index < K::SceneManager::scenes.size()) 
				{
					std::string location = K::SceneManager::scenes[K::SceneManager::index];
					std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
					K::Deserializer deserializer = K::Deserializer(K::SceneManager::currentScene, K::SceneManager::scenes[index]);
				}
			}
			else
			{
				K::SceneManager::currentScene->Init();
			}
			K::SceneManager::loadScene = false;
			K::SceneManager::index = -1;
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