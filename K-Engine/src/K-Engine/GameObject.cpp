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
		std::srand(std::chrono::system_clock::now().time_since_epoch().count());
		this->g_Index = std::rand();
		K::Editor::GetCurrentScene()->Attach(this);
		std::cout << name << " GameObject Created" << std::endl;
	}

	GameObject::GameObject(const char* name, K::Transform* transform, int index)
	{
		this->name = (char*)name;
		this->transform = transform;
		this->material = K::Editor::GetMaterial();
		this->g_Index = index;
		K::Editor::GetCurrentScene()->Attach(this);
		std::cout << name << " GameObject Created" << std::endl;
	}

	GameObject::~GameObject() 
	{
		std::cout << "Begin GameObject Destruction..." << std::endl;
		std::cout << "Begin Component Destruction..." << std::endl;
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			delete this->components[i];
		}
		this->components.clear();
		std::cout << "End Component Destruction..." << std::endl;
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
			if (ImGui::TreeNode("World Space Transforms")) 
			{
				ImGui::DragFloat3("Position:", &Editor::GetSelectedGameObject()->GetTransform()->position->x);
				ImGui::DragFloat3("Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->rotation->x);
				ImGui::DragFloat3("Scale:", &Editor::GetSelectedGameObject()->GetTransform()->scale->x);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Local Space Transforms"))
			{
				ImGui::DragFloat3("Local Position:", &Editor::GetSelectedGameObject()->GetTransform()->localPosition->x);
				ImGui::DragFloat3("Local Rotation:", &Editor::GetSelectedGameObject()->GetTransform()->localRotation->x);
				ImGui::DragFloat3("Local Scale:", &Editor::GetSelectedGameObject()->GetTransform()->localScale->x);
				ImGui::TreePop();
			}
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
		if (parent->children.size() > 0)
		{
			for (int i : parent->children)
			{
				if (gameObject == K::Editor::GetCurrentScene()->GetGameObjects().at(i))
				{
					return true;
				}
				else 
				{
					if (CheckForGameObjectInChildren(K::Editor::GetCurrentScene()->GetGameObjects().at(i), gameObject))
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

	void GameObject::UpdateToWorld() 
	{
		if (this->parent == nullptr) 
		{
			
		}
		else 
		{
			*this->GetTransform()->localScale = *this->GetTransform()->scale / *this->parent->GetTransform()->scale;
			*this->GetTransform()->localRotation = *this->GetTransform()->rotation - *this->parent->GetTransform()->rotation;
			*this->GetTransform()->localPosition = *this->GetTransform()->position;
			K::Matrix4x4 invert = K::QuickInverse(this->parent->GetTransform()->modelMatrix);
			K::MultiplyMatrixVector(*this->GetTransform()->position, *this->GetTransform()->localPosition, invert);
		}
	}

	void GameObject::AddChild(K::GameObject* index)
	{
		*index->GetTransform()->localScale /= *this->GetTransform()->scale;
		*index->GetTransform()->localRotation -= *this->GetTransform()->rotation;
		K::Matrix4x4 invert = K::QuickInverse(this->GetTransform()->modelMatrix);
		K::MultiplyMatrixVector(*index->GetTransform()->position, *index->GetTransform()->localPosition, invert);
		for (auto temp : K::Editor::GetCurrentScene()->GetGameObjects())
		{
			if (temp.second == index) 
			{
				this->children.push_back(temp.first);
				break;
			}
		}
	}

	void GameObject::RemoveChild(K::GameObject* index) 
	{
		K::Vector3 localScaleInWorldSpace = *index->GetTransform()->localScale * *this->GetTransform()->scale;
		K::Vector3 localRotationInWorldSpace = *index->GetTransform()->localRotation + *this->GetTransform()->rotation;
		K::Vector3 localPositionInWorldSpace;
		K::MultiplyMatrixVector(*index->GetTransform()->localPosition, localPositionInWorldSpace, this->GetTransform()->modelMatrix);
		*index->GetTransform()->scale = localScaleInWorldSpace;
		*index->GetTransform()->rotation = localRotationInWorldSpace;
		*index->GetTransform()->position = localPositionInWorldSpace;
		int j = 0;
		for (int i : this->children)
		{
			if (K::Editor::GetCurrentScene()->GetGameObjects()[i] == index)
			{
				this->children.erase(this->children.begin() + j);
				break;
			}
			j++;
		}
	}

	void GameObject::SetIndex(int index) 
	{
		this->g_Index = index;
	}

	int GameObject::GetIndex() 
	{
		return this->g_Index;
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