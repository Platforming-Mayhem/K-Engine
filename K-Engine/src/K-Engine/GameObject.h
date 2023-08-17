#pragma once
#include "build.h"
#include "Shader.h"
#include "Mesh.h"
#include "Component.h"
#include "Window.h"

namespace K 
{
	class K_API GameObject
	{
	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		std::string name;
		K::Shader* shader;
		K::Mesh* mesh;
		K::Transform* transform;
		std::vector<K::Component*> components;
		ImGui::FileBrowser file;
	public:

		bool meshUpdate = false;

		GameObject(const char* name, Transform* transform, K::Shader* shader);

		virtual ~GameObject();

		void PassTransformationMatrix();

		void Update();

		void UpdateEditor();

		const char* GetName() 
		{
			return this->name.c_str();
		}

		void SetName(std::string name) 
		{
			this->name = name;
		}

		K::Mesh* GetMesh() 
		{
			return this->mesh;
		}

		K::Transform* GetTransform() 
		{
			return this->transform;
		}

		void AddComponent(K::Component* component)
		{
			component->SetParent(this);
			component->Init();
			this->components.push_back(component);
		}

		void SetPosition(K::Vector3 position)
		{
			GetTransform()->position = &position;
		}

		void SetRotation(K::Vector3 rotation)
		{
			GetTransform()->rotation = &rotation;
		}

		void SetScale(K::Vector3 scale)
		{
			GetTransform()->scale = &scale;
		}

		K::Component* GetComponent(int index) 
		{
			return this->components[index];
		}

		int GetNumberOfComponents() 
		{
			return this->components.size();
		}

		void Bind();

		void Unbind();

		void Render();

		bool IsSelected(K::Window* window);
	};
}