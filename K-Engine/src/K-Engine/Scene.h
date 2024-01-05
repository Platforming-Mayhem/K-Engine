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
			for (K::GameObject* temp : this->gameObjects) 
			{
				delete temp;
			}
			this->gameObjects.clear();
			this->gameObjects.shrink_to_fit();
		}

		K::GameObject* CreateGameObject(const char* name, K::Transform* transform, K::Material* mat) 
		{
			K::GameObject* temp = new K::GameObject(name, transform, mat);
			this->Attach(temp);
			return temp;
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