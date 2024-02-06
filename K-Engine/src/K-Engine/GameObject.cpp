#include "GameObject.h"
#include "InputManager.h"
#include "Editor.h"

namespace K 
{
	GameObject::GameObject(const char* name, K::Transform* transform)
	{
		this->name = (char*)name;
		this->transform = transform;
		this->material = K::Editor::GetMaterial();
		K::Editor::GetCurrentScene()->Attach(this);
		std::cout << name << " GameObject Created" << std::endl;
	}

	GameObject::~GameObject() 
	{
		std::cout << "Begin GameObject Destruction..." << std::endl;
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			delete this->components[i];
		}
		this->components.clear();
		this->children.clear();
		this->material = nullptr;
		delete this->transform;
		std::cout << "End GameObject Destruction..." << std::endl;
	}

	void GameObject::Init() 
	{
		for (K::Component* c : this->components)
		{
			std::cout << "Initializing " << c->GetName() << std::endl;
			c->Init();
		}
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

	void GameObject::SetParent(K::GameObject* newParent) 
	{
		/*if (this->parent == nullptr)
		{
			std::cout << "GameObject:" << this->GetName() << ",Set Parent:" << newParent->GetName() << std::endl;
			std::cout << "GameObject:" << newParent->GetName() << ",Set Child:" << this->GetName() << std::endl;
		}
		else 
		{
			std::cout << "GameObject:" << this->GetName() << this->parent->GetName() << ",Set Parent:" << newParent->GetName() << std::endl;
			std::cout << "GameObject:" << newParent->GetName() << ",Set Child:" << this->GetName() << std::endl;
		}*/
		//std::cout << this->GetName() << std::endl;
		//std::cout << newParent->GetName() << std::endl;
		if (this->parent != nullptr) 
		{
			this->parent->RemoveChild(this);
		}
		newParent->AddChild(this);
		this->parent = newParent;
	}

	void GameObject::AddChild(K::GameObject* child)
	{
		this->children.push_back(child);
	}

	void GameObject::RemoveChild(K::GameObject* child) 
	{
		this->children.erase(std::find(this->children.begin(), this->children.end(), child));
	}

	K::GameObject* GameObject::GetChild(int index) 
	{
		return this->children.at(index);
	}

	int GameObject::GetNumberOfChildren() 
	{
		return this->children.size();
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