#pragma once
#include "Scene.h"
#include "Serializer.h"

namespace K 
{
	class K_API SceneManager
	{
	private:
		static std::vector<std::string> scenes;
		static bool loadNextScene;
	public:

		static K::Scene* currentScene;

		SceneManager(unsigned int resource);

		~SceneManager();

		int GetNumberOfScenes();

		void AddScene(std::string name);

		static void Update();

		static void LoadScene(int index);

		static void LoadNextScene();

		void DeleteScene(int index);

		static const char* GetSceneName(int index);

		void SaveSceneManager();
	};
}