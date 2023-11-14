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
	public:
		K::Matrix4x4 cameraMatrix;

		Scene(std::string newSceneName) 
		{
			this->sceneName = newSceneName;
		}

		~Scene() 
		{

		}

		void Attach(K::GameObject* gameObject)
		{
			this->gameObjects.push_back(gameObject);
		}

		void Delete(K::GameObject* gameObject) 
		{
			for (int i = 1; i < this->GetNumberOfObjects(); i++) 
			{
				if (gameObject == this->gameObjects[i]) 
				{
					this->gameObjects.erase(this->gameObjects.begin() + i);
					delete gameObject;
				}
			}
		}

		void CreateEmptyScene() 
		{
			int size = this->GetNumberOfObjects();
			for (int i = 1; i < size; i++)
			{
				K::GameObject* g = this->gameObjects.back();
				this->gameObjects.pop_back();
				delete g;
			}
		}

		int GetNumberOfObjects() 
		{
			return this->gameObjects.size();
		}

		std::vector <K::GameObject*> GetGameObjects()
		{
			return this->gameObjects;
		}

		std::string* GetSceneName() 
		{
			return &this->sceneName;
		}

		void Render(K::Material* mat) 
		{
			for (auto i : this->gameObjects)
			{
				i->Bind();
				i->PassTransformationMatrix(mat);
				i->Update();
				i->Unbind();
			}
		}
	};
}