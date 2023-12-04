#include "GameObject.h"
#include "Editor.h"
#include "InputManager.h"

namespace K 
{
	GameObject::GameObject(const char* name, K::Transform* transform, K::Material* material)
	{
		this->name = (char*)name;
		this->transform = *transform;
		this->material = material;
	}

	GameObject::~GameObject() 
	{
		std::cout << "Begin GameObject Destruction..." << std::endl;
		if (this->GetNumberOfComponents() > 0) 
		{
			for (int i = 0; i < this->components.size(); i++)
			{
				delete this->components[i];
			}
			this->components.clear();
		}
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
		ImGui::Separator();
		if (ImGui::CollapsingHeader("GameObject Settings"))
		{
			ImGui::DragFloat3("Position:", &Editor::GetSelectedGameObject()->GetTransform()->position->x);
			ImGui::DragFloat3("Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->rotation->x);
			ImGui::DragFloat3("Scale:", &Editor::GetSelectedGameObject()->GetTransform()->scale->x);
		}
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->UpdateEditor();
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