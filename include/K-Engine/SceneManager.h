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
		static bool loadScene;
		static bool quit;
		static int index;
	public:

		static K::Scene* currentScene;

		SceneManager(unsigned int resource);

		SceneManager(std::string path);

		~SceneManager();

		int GetNumberOfScenes();

		static bool IsLoadingNextScene();

		void AddScene(std::string name);

		static bool Update();

		static void Quit();

		static void LoadScene(int newIndex);

		static void LoadNextScene();

		void DeleteScene(int index);

		static const char* GetSceneName(int index);

		void SaveSceneManager();
	};
}