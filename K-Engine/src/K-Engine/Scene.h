#pragma once
#include "build.h"
#include "GameObject.h"

namespace K 
{
	class K_API Scene
	{
	private:
		std::string sceneName;
		std::vector<K::GameObject*> gameObjects;
		int numberOfGameObjects = 0;
	public:
		K::Matrix4x4 cameraMatrix;

		Scene(std::string newSceneName) 
		{
			this->sceneName = newSceneName;
		}

		void Attach(K::GameObject* gameObject)
		{
			this->gameObjects.push_back(gameObject);
		}

		int GetNumberOfObjects() 
		{
			return this->numberOfGameObjects;
		}

		std::vector <K::GameObject*> GetGameObjects()
		{
			return gameObjects;
		}

		std::string* GetSceneName() 
		{
			return &this->sceneName;
		}

		void Render(K::Material* mat) 
		{
			int number = 0;
			for (auto i : gameObjects)
			{
				i->Bind();
				i->PassTransformationMatrix(mat);
				i->Update();
				i->Unbind();
				number++;
			}
			this->numberOfGameObjects = number;
		}
	};
}