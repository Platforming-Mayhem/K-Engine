#pragma once
#include "K-Engine/build.h"
#include "K-Engine/Material.h"
#include "K-Engine/Transform.h"
#include "K-Engine/Component.h"
#include "K-Engine/Window.h"

namespace K 
{
	struct Layer 
	{
		enum class LayerType { Ground = 0, Player = 1, Enemy = 2};

		Layer(LayerType layer)
		{
			this->layer = (int)layer;
		}
		int layer = 0;
	};

	class K_API GameObject
	{
	private:
		std::string name;
		K::Transform* transform = new K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new K::Vector3(0.0f, 0.0f, 0.0f), new K::Vector3(1.0f, 1.0f, 1.0f));
		std::vector<K::Component*> components;
		K::Material* material = nullptr;
		int g_Index;
	public:

		std::vector<int> children;

		K::GameObject* parent = nullptr;

		int layer = (int)K::Layer::LayerType::Ground;

		GameObject(const char* name, K::Transform* transform, int index);

		GameObject(const char* name, K::Transform* transform);

		GameObject(const GameObject&);

		~GameObject();

		void Init();

		void RenderInit();

		void Update();

		void UpdateEditor();

		void RenderBind();

		void Render();

		void RenderUnbind();

		void PassTransformationMatrix();

		void Bind();

		void Unbind();

		bool SetParent(K::GameObject* newParent);

		void RemoveChild(K::GameObject* index);

		void AddChild(K::GameObject* index);

		void SetIndex(int index);

		int GetIndex();

		bool CheckForGameObjectInChildren(K::GameObject* parent, K::GameObject* gameObject);

		const char* GetName() 
		{
			return this->name.c_str();
		}

		void SetName(std::string name) 
		{
			this->name = name;
		}

		K::Transform* GetTransform() 
		{
			return this->transform;
		}

		void AddComponent(K::Component* component)
		{
			component->SetParent(this);
			this->components.push_back(component);
		}

		void SetPosition(K::Vector3 position)
		{
			*GetTransform()->position = position;
		}

		void SetRotation(K::Vector3 rotation)
		{
			*GetTransform()->rotation = rotation;
		}

		void SetScale(K::Vector3 scale)
		{
			*GetTransform()->scale = scale;
		}

		void SetMaterial(K::Material* material, bool deleteMaterial = true) 
		{
			if(this->material != material && this->material != nullptr && deleteMaterial)
				delete this->material;
			this->material = material;
		}

		K::Component* GetComponent(int index) 
		{
			if (this->components[index] != nullptr)
				return this->components[index];
			else 
			{
				std::cout << "Component no longer exists" << std::endl;
				return nullptr;
			}
		}

		K::Component* GetComponentOfType(const char* component) 
		{
			for (int i = 0; i < this->GetNumberOfComponents(); i++) 
			{
				if (this->GetComponent(i)->GetName() == component) 
				{
					return this->GetComponent(i);
				}
			}
			return nullptr;
		}

		K::Material* GetMaterial() 
		{
			return this->material;
		}

		int GetNumberOfComponents() 
		{
			return this->components.size();
		}
	};
}