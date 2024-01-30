#include "SceneManager.h"

namespace K
{
	SceneManager::SceneManager()
	{
		this->currentScene = new K::Scene("Untitled");
	}

	void SceneManager::LoadScene(int index) 
	{
		this->currentScene = this->scenes[index];
	}

	void SceneManager::AddScene(K::Scene* scene)
	{
		this->scenes.push_back(scene);
	}
}