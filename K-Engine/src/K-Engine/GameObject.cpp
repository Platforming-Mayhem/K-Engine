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
		this->g_Index = K::Editor::GetCurrentScene()->GetNumberOfObjects();
		K::Editor::GetCurrentScene()->Attach(this);
		std::cout << name << " GameObject Created" << std::endl;
	}

	GameObject::~GameObject() 
	{
		std::cout << "Begin GameObject Destruction..." << std::endl;
		this->children.clear();
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			delete this->components[i];
		}
		this->components.clear();
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
		ImGui::Text("Index: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(this->g_Index).c_str());
		ImGui::Text(Editor::GetSelectedGameObject()->GetName());
		ImGui::Text(std::to_string(this->layer).c_str());
		ImGui::Separator();
		if (ImGui::CollapsingHeader("GameObject Settings"))
		{
			ImGui::DragFloat3("Position:", &Editor::GetSelectedGameObject()->GetTransform()->position->x);
			ImGui::DragFloat3("Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->rotation->x);
			ImGui::DragFloat3("Scale:", &Editor::GetSelectedGameObject()->GetTransform()->scale->x);

			ImGui::DragFloat3("Local Position:", &Editor::GetSelectedGameObject()->GetTransform()->localPosition->x);
			ImGui::DragFloat3("Local Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->localRotation->x);
			ImGui::DragFloat3("Local Scale:", &Editor::GetSelectedGameObject()->GetTransform()->localScale->x);
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

	bool GameObject::CheckForGameObjectInChildren(K::GameObject* parent, K::GameObject* gameObject)
	{
		if (parent->GetNumberOfChildren() > 0) 
		{
			for (int i = 0; i < parent->children.size(); i++)
			{
				if (gameObject == parent->GetChild(i))
				{
					return true;
				}
				else 
				{
					if (CheckForGameObjectInChildren(parent->GetChild(i), gameObject)) 
					{
						return true;
					}
				}
			}
		}
		else 
		{
			return false;
		}
	}

	bool GameObject::SetParent(K::GameObject* newParent) 
	{
		if (newParent == nullptr) 
		{
			if (this->parent != nullptr)
			{
				this->parent->RemoveChild(this);
			}
			this->parent = nullptr;
			return true;
		}
		else 
		{
			if (CheckForGameObjectInChildren(this, newParent))
			{
				return false;
			}
			else
			{
				if (this->parent != nullptr)
				{
					this->parent->RemoveChild(this);
				}
				this->parent = newParent;
				newParent->AddChild(this);
				return true;
			}
		}
	}

	void GameObject::AddChild(K::GameObject* index)
	{
		*index->GetTransform()->localScale /= *this->GetTransform()->scale;
		*index->GetTransform()->localRotation -= *this->GetTransform()->rotation;
		//POSITION NEEDS TO BE FIXED, I THINK
		*index->GetTransform()->localPosition -= *this->GetTransform()->position;
		for (int i = 0; i < K::Editor::GetCurrentScene()->GetNumberOfObjects(); i++) 
		{
			if (K::Editor::GetCurrentScene()->GetGameObjects()[i] == index) 
			{
				this->children.push_back(i);
				break;
			}
		}
	}

	void GameObject::RemoveChild(K::GameObject* index) 
	{
		index->GetTransform()->localScale->ResetScale();
		index->GetTransform()->localRotation->Reset();
		index->GetTransform()->localPosition->Reset();
		for (int i = 0; i < this->children.size(); i++)
		{
			if (K::Editor::GetCurrentScene()->GetGameObjects()[this->children[i]] == index)
			{
				this->children.erase(this->children.begin() + i);
				break;
			}
		}
	}

	K::GameObject* GameObject::GetChild(int index) 
	{
		return K::Editor::GetCurrentScene()->GetGameObjects()[this->children[index]];
	}

	int GameObject::GetIndex() 
	{
		return this->g_Index;
	}

	int GameObject::GetChildIndex(int index)
	{
		return this->children[index];
	}

	int GameObject::GetNumberOfChildren() 
	{
		return this->children.size();
	}

	void GameObject::PassTransformationMatrix()
	{
		if (this->parent == nullptr) 
			this->GetTransform()->PassModelMatrix();
		else 
			this->GetTransform()->PassModelMatrix(this->parent->GetTransform());
		glUniformMatrix4fv(glGetUniformLocation(this->material->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &this->GetTransform()->modelMatrix.m[0][0]);
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