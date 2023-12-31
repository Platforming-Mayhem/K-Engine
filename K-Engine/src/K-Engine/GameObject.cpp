#include "GameObject.h"
#include "InputManager.h"
#include "Editor.h"

namespace K 
{
	GameObject::GameObject(const char* name, K::Transform* transform, K::Material* material)
	{
		this->name = (char*)name;
		this->transform = transform;
		this->material = material;
		std::cout << name << " GameObject Created" << std::endl;
	}

	GameObject::~GameObject() 
	{
		std::cout << "Begin GameObject Destruction..." << std::endl;
		for (K::Component* comp : this->components)
		{
			delete comp;
		}
		delete this->transform;
		this->components.clear();
		this->components.shrink_to_fit();
		this->material = nullptr;
		std::cout << "End GameObject Destruction..." << std::endl;
	}

	void GameObject::Update()
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Update();
		}
	}

	void GameObject::UpdateEditor()
	{
		ImGui::Separator();
		//INPUT TEXT DOESN'T WORK WHEN THE STRING IS EMPTY
		ImGui::InputText("Name:", &(this->name));
		ImGui::Text(Editor::GetSelectedGameObject()->GetName());
		ImGui::Text(std::to_string(this->layer).c_str());
		ImGui::Separator();
		if (ImGui::CollapsingHeader("GameObject Settings"))
		{
			ImGui::DragFloat3("Position:", &Editor::GetSelectedGameObject()->GetTransform()->position->x);
			ImGui::DragFloat3("Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->rotation->x);
			ImGui::DragFloat3("Scale:", &Editor::GetSelectedGameObject()->GetTransform()->scale->x);
		}
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->components[i]->UpdateEditor();
			if (ImGui::Button(("Delete Component##" + std::to_string(i)).c_str()))
			{
				std::cout << "Delete Component..." << std::endl;
				delete this->GetComponent(i);
				this->components.erase(this->components.begin() + i);
				this->components.shrink_to_fit();
			}
		}
	}

	void GameObject::PassTransformationMatrix()
	{
		K::GameObject::GetTransform()->PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &K::GameObject::GetTransform()->modelMatrix.m[0][0]);
	}

	void GameObject::Bind() 
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Bind();
		}
	}

	void GameObject::Unbind() 
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Unbind();
		}
	}
}