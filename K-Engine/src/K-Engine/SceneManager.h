#pragma once
#include "Scene.h"
#include "Serializer.h"

namespace K 
{
	class K_API SceneManager
	{
	private:
		std::vector<std::string> scenes;
	public:

		static K::Scene* currentScene;

		SceneManager(unsigned int resource);

		~SceneManager();

		int GetNumberOfScenes();

		void AddScene(std::string name);

		void LoadScene(int index);

		const char* GetSceneName(int index);

		void SaveSceneManager();
	};
}