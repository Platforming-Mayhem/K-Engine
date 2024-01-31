#pragma once
#include "build.h"
#include "GameObject.h"
#include "Time.h"

namespace K 
{
	class K_API Scene
	{
	private:
		std::string location;
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
			for (K::GameObject* temp : this->gameObjects) 
			{
				delete temp;
			}
			this->gameObjects.clear();
			this->gameObjects.shrink_to_fit();
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
			this->gameObjects.push_back(gameObject);
		}

		void Init()
		{
			if (!this->gameObjects.empty()) 
			{
				for (K::GameObject* g : this->gameObjects)
				{
					std::cout << "Initializing " << g->GetName() << std::endl;
					g->Init();
				}
			}
			K::Time::startTime = glfwGetTime();
			K::Time::endTime = K::Time::startTime;
		}

		void Delete(K::GameObject* gameObject) 
		{
			#if _DEBUG
			for (int i = 1; i < this->GetNumberOfObjects(); i++)
			{
				if (gameObject == this->gameObjects[i]) 
				{
					std::cout << this->gameObjects[i]->GetName() << std::endl;
					delete this->gameObjects[i];
					this->gameObjects.erase(this->gameObjects.begin() + i);
					this->gameObjects.shrink_to_fit();
					break;
				}
			}
			#else
			for (int i = 0; i < this->GetNumberOfObjects(); i++)
			{
				if (gameObject == this->gameObjects[i])
				{
					std::cout << this->gameObjects[i]->GetName() << std::endl;
					delete this->gameObjects[i];
					this->gameObjects.erase(this->gameObjects.begin() + i);
					this->gameObjects.shrink_to_fit();
					break;
				}
			}
			#endif
		}

		void CreateEmptyScene() 
		{
			int size = this->GetNumberOfObjects();
			if (size > 0) 
			{
				#if _DEBUG
				for (int i = 1; i < this->GetNumberOfObjects(); i++)
				{
					std::cout << this->gameObjects[i]->GetName() << std::endl;
					delete this->gameObjects[i];
				}
				this->gameObjects.erase(this->gameObjects.begin() + 1, this->gameObjects.end());
				#else
				for (int i = 0; i < this->GetNumberOfObjects(); i++)
				{
					std::cout << this->gameObjects[i]->GetName() << std::endl;
					delete this->gameObjects[i];
				}
				this->gameObjects.erase(this->gameObjects.begin(), this->gameObjects.end());
				#endif
			}
			this->gameObjects.shrink_to_fit();
			this->sceneName = "Untitled*";
		}

		int GetNumberOfObjects() 
		{
			int size = this->gameObjects.size();
			return size;
		}

		std::vector <K::GameObject*> GetGameObjects()
		{
			return this->gameObjects;
		}

		const char* GetSceneName() 
		{
			return this->sceneName.c_str();
		}

		void Render() 
		{
			K::Time::startTime = glfwGetTime();
			for (int i = 0; i < this->GetNumberOfObjects(); i++)
			{
				this->GetGameObjects()[i]->Bind();
				this->GetGameObjects()[i]->PassTransformationMatrix();
				this->GetGameObjects()[i]->Update();
				this->GetGameObjects()[i]->Unbind();
			}
			K::Time::endTime = K::Time::startTime;
		}
	};
}