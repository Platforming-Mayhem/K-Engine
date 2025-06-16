#include "K-Engine/Scene.h"
#include "K-Engine/Time.h"

namespace K 
{
	Scene::Scene(std::string newSceneName)
	{
		this->sceneName = newSceneName;
	}

	Scene::~Scene() 
	{
		for (auto i : this->gameObjects)
		{
			delete i.second;
		}
		this->gameObjects.clear();
	}

	void Scene::SetLocation(std::string location)
	{
		this->location = location;
	}

	void Scene::SetSceneName(std::string name)
	{
		this->sceneName = name;
	}

	const char* Scene::GetLocation() 
	{
		return this->location.c_str();
	}

	void Scene::Attach(K::GameObject* gameObject)
	{
		this->gameObjects.insert({ gameObject->GetIndex() ,gameObject });
	}

	void Scene::Init() 
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

	void Scene::Delete(K::GameObject* gameObject)
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

	void Scene::CreateEmptyScene() 
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

	void Scene::RenameScene(std::string newSceneName)
	{
		if (!newSceneName.contains(".JAWS"))
		{
			this->sceneName = "scenes/" + newSceneName + ".JAWS";
			this->location = "scenes/" + newSceneName + ".JAWS";
		}
		else 
		{
			this->sceneName = "scenes/" + newSceneName;
			this->location = "scenes/" + newSceneName;
		}
	}

	int Scene::GetNumberOfObjects() 
	{
		return this->gameObjects.size();
	}

	std::map<int, K::GameObject*> Scene::GetGameObjects() 
	{
		return this->gameObjects;
	}

	const char* Scene::GetSceneName() 
	{
		return this->sceneName.c_str();
	}

	void Scene::GameLoop(K::GameObject* temp)
	{
		temp->Bind();
		temp->Update();
		temp->Unbind();
	}

	void Scene::RenderLoop(K::GameObject* temp) 
	{
		temp->PassTransformationMatrix();
		temp->RenderBind();
		temp->Render();
		temp->RenderUnbind();
	}

	void Scene::RenderLoopNewMaterial(K::Material* mat)
	{
		for (auto temp : this->GetGameObjects())
		{
			K::Material* tempMat = temp.second->GetMaterial();
			temp.second->SetMaterial(mat, false);
			int index = temp.second->GetIndex();
			temp.second->PassTransformationMatrix();
			temp.second->RenderBind();
			if(tempMat != nullptr)
				glUniform1i(temp.second->GetMaterial()->GetShader()->GetUniform("index"), index);
			temp.second->Render();
			temp.second->RenderUnbind();
			temp.second->SetMaterial(tempMat, false);
		}
	}

	void Scene::Render() 
	{
		K::Time::startTime = (float)glfwGetTime();
		for (auto temp : this->GetGameObjects())
		{
			this->GameLoop(temp.second);
			this->RenderLoop(temp.second);
		}
		K::Time::deltaTimeValue = K::Time::startTime - K::Time::endTime;
		K::Time::endTime = K::Time::startTime;
	}
}