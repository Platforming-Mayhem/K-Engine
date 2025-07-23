#include "K-Engine/SceneManager.h"
#include "K-Engine/Editor.h"

namespace K
{
	std::vector<std::string> K::SceneManager::scenes;
	K::Scene* K::SceneManager::currentScene;
	bool K::SceneManager::loadNextScene = false;
	bool K::SceneManager::loadScene = false;
	bool K::SceneManager::quit = false;
	int K::SceneManager::index = -1;
	std::string K::SceneManager::relativeFilePath;

	SceneManager::SceneManager(std::string path)
	{
		K::SceneManager::LoadBuildMenu(path);

		K::SceneManager::currentScene = new K::Scene("Untitled*");
	}

	void SceneManager::LoadBuildMenu(std::string path)
	{
		if (!path.empty() && !ASSET_DIR.empty())
		{
			relativeFilePath = path;
			std::ifstream inFile(ASSET_DIR + relativeFilePath);
			if (inFile.good())
			{
				std::string line;
				while (std::getline(inFile, line))
				{
					this->AddScene(line.c_str());
				}
			}
		}
	}

	SceneManager::~SceneManager() 
	{
		this->scenes.clear();
		delete this->currentScene;
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

	void SceneManager::Quit() 
	{
		K::SceneManager::quit = true;
	}

	bool SceneManager::Update() 
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
					K::SceneManager::currentScene->SetSceneIndex(K::SceneManager::index);
					K::Deserializer deserializer = K::Deserializer(K::SceneManager::currentScene, K::SceneManager::scenes[K::SceneManager::index]);
				}
			}
			else
			{
				K::SceneManager::currentScene->Init();
			}
			K::SceneManager::loadScene = false;
			K::SceneManager::index = -1;
		}
		if (K::SceneManager::quit) 
		{
			return true;
		}
		else 
		{
			return false;
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
		outFile.open(ASSET_DIR + relativeFilePath, std::ofstream::trunc);
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