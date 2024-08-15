#pragma once
#include "build.h"
#include "GameObject.h"
#include "Camera.h"
#include "Time.h"

namespace K 
{
	class K_API Scene
	{
	private:
		std::string location;
		std::string sceneName;
		std::map<int, K::GameObject*> gameObjects;
	public:

		Scene(std::string newSceneName) 
		{
			this->sceneName = newSceneName;
		}

		~Scene() 
		{
			for (auto i : this->gameObjects) 
			{
				delete i.second;
			}
			this->gameObjects.clear();
		}

		void SetLocation(std::string location) 
		{
			this->location = location;
		}

		void SetSceneName(std::string name) 
		{
			this->sceneName = name;
		}

		const char* GetLocation() 
		{
			return this->location.c_str();
		}

		void Attach(K::GameObject* gameObject)
		{
			this->gameObjects.insert({ gameObject->GetIndex() ,gameObject });
		}

		void Init()
		{
			if (!this->gameObjects.empty()) 
			{
				for (auto g : this->gameObjects)
				{
					g.second->Init();
				}
				for (auto g : this->gameObjects)
				{
					g.second->RenderInit();
				}
			}
			K::Time::startTime = glfwGetTime();
			K::Time::endTime = K::Time::startTime;
		}

		void Delete(K::GameObject* gameObject) 
		{
			for (auto temp : this->gameObjects)
			{
				if (gameObject == temp.second)
				{
					//std::cout << temp.second->GetName() << std::endl;
					int index = temp.first;
					delete temp.second;
					this->gameObjects.erase(index);
					break;
				}
			}
		}

		void CreateEmptyScene() 
		{
			int size = this->GetNumberOfObjects();
			if (size > 0) 
			{
				for (auto temp : this->gameObjects)
				{
					//std::cout << temp.second->GetName() << std::endl;
					delete temp.second;
				}
				this->gameObjects.erase(this->gameObjects.begin(), this->gameObjects.end());
			}
			this->sceneName = "Untitled*";
			this->location = "scenes/" + this->sceneName;
		}

		void RenameScene(std::string newSceneName) 
		{
			this->sceneName = "scenes/" + newSceneName;
			this->location = "scenes/" + newSceneName;
		}

		int GetNumberOfObjects() 
		{
			return this->gameObjects.size();
		}

		std::map <int, K::GameObject*> GetGameObjects()
		{
			return this->gameObjects;
		}

		const char* GetSceneName() 
		{
			return this->sceneName.c_str();
		}

		void GameLoop() 
		{
			for (auto temp : this->GetGameObjects())
			{
				temp.second->Bind();
				temp.second->Update();
				temp.second->Unbind();
			}
		}

		void RenderLoop() 
		{
			for (auto temp : this->GetGameObjects())
			{
				temp.second->PassTransformationMatrix();
				temp.second->RenderBind();
				temp.second->Render();
				temp.second->RenderUnbind();
			}
		}

		void Render() 
		{
			K::Time::startTime = glfwGetTime();
			this->GameLoop();
			this->RenderLoop();
			K::Time::endTime = K::Time::startTime;
		}
	};
}