#pragma once
#include "Scene.h"

namespace K 
{
	class K_API SceneManager
	{
	private:
		std::vector<K::Scene*> scenes;
	public:

		K::Scene* currentScene;

		SceneManager();

		void AddScene(K::Scene* scene);

		void LoadScene(int index);
	};
}