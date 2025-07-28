#pragma once
#include "build.h"
#include "GameObject.h"

namespace K 
{
	class K_API Scene
	{
	private:
		std::string location;
		std::string sceneName;
		int sceneIndex = -1;
		std::map<int, K::GameObject*> gameObjects;
	public:

		bool isPaused = false;

		Scene(std::string newSceneName);

		~Scene();

		void SetSceneIndex(int index);

		int GetBuildIndex();

		void SetLocation(std::string location);

		void SetSceneName(std::string name);

		std::string GetLocation();

		void Attach(K::GameObject* gameObject);

		void Init();

		void Delete(K::GameObject* gameObject);

		void CreateEmptyScene();

		void RenameScene(std::string newSceneName);

		int GetNumberOfObjects();

		std::map <int, K::GameObject*> GetGameObjects();

		std::string GetSceneName();

		void GameLoop(K::GameObject* temp);

		void RenderLoop(K::GameObject* temp);

		void RenderLoopNewMaterial(K::Material* mat);

		void Render();
	};
}