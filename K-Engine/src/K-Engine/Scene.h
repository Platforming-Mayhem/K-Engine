#pragma once
#include "build.h"
#include "GameObject.h"
#include "Time.h"

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

		void Init()
		{
			for (K::GameObject* g : this->gameObjects) 
			{
				std::cout << "Initializing " << g->GetName() << std::endl;
				g->Init();
			}
			K::Time::startTime = glfwGetTime();
			K::Time::endTime = K::Time::startTime;
		}

		void Delete(K::GameObject* gameObject) 
		{
			int size = this->GetNumberOfObjects();
			int index = -1;
			for (int i = 1; i < size; i++)
			{
				if (gameObject == this->gameObjects[i]) 
				{
					delete this->gameObjects[i];
					index = i;
				}
			}
			if (index != -1) 
			{
				this->gameObjects.erase(this->gameObjects.begin() + index);
				this->gameObjects.shrink_to_fit();
			}
		}

		void CreateEmptyScene() 
		{
			int size = this->GetNumberOfObjects();
			if (size > 0) 
			{
				for (int i = 1; i < size; i++)
				{
					delete this->gameObjects[i];
				}
				this->gameObjects.erase(this->gameObjects.begin() + 1, this->gameObjects.end());
			}
			this->gameObjects.shrink_to_fit();
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

		void Render() 
		{
			K::Time::startTime = glfwGetTime();
			for (auto i : this->gameObjects)
			{
				i->Bind();
				i->PassTransformationMatrix();
				i->Update();
				i->Unbind();
			}
			K::Time::endTime = K::Time::startTime;
		}
	};
}