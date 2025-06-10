#include "K-Engine/GameObject.h"
#include "K-Engine/InputManager.h"
#include "K-Engine/Editor.h"

namespace K 
{
	//GENERATE INDEX VALUE WITH RANDOM INT

	GameObject::GameObject(const char* name, K::Transform* transform)
	{
		this->name = (char*)name;
		this->transform = transform;
		std::srand(std::chrono::system_clock::now().time_since_epoch().count());
		this->g_Index = std::rand();
		K::Editor::GetCurrentScene()->Attach(this);
		//std::cout << name << " GameObject Created" << std::endl;
	}

	//SET CUSTOM INDEX VALUE

	GameObject::GameObject(const char* name, K::Transform* transform, int index)
	{
		this->name = (char*)name;
		this->transform = transform;
		this->g_Index = index;
		K::Editor::GetCurrentScene()->Attach(this);
		//std::cout << name << " GameObject Created" << std::endl;
	}

	//DEEP COPY

	GameObject::GameObject(const GameObject &other) : name(other.name)
	{
		this->name = other.name;
		this->transform = new K::Transform();
		/**this->transform->position = *other.transform->position;
		*this->transform->rotation = *other.transform->rotation;
		*this->transform->scale = *other.transform->scale;
		*this->transform->localPosition = *other.transform->localPosition;
		*this->transform->localRotation = *other.transform->localRotation;
		*this->transform->localScale = *other.transform->localScale;*/
		this->g_Index = other.g_Index;
		/*for (int i = 0; i < other.components.size(); i++)
		{
			K::Component* comp = K::Editor::lst().at(other.components[i]->GetName())->create();
			this->AddComponent(comp);
			std::string allValues = other.components[i]->GetPropertyValues();
			std::string value;
			int index = 0;
			for (int i = 0; i < allValues.size(); i++)
			{
				if (allValues[i] == ',')
				{
					comp->SetPropertyValues(value.c_str(), index);
					value = "";
					index++;
				}
				else
				{
					value += allValues[i];
				}
			}
			if (value != "")
			{
				comp->SetPropertyValues(value.c_str(), index);
			}
			comp->Init();
			comp->RenderInit();
		}*/
		K::Editor::GetCurrentScene()->Attach(this);
	}

	GameObject::~GameObject() 
	{
		//std::cout << "Begin GameObject Destruction..." << std::endl;
		//std::cout << "Begin Component Destruction..." << std::endl;
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			delete this->components[i];
		}
		this->components.clear();
		//std::cout << "End Component Destruction..." << std::endl;
		delete this->material;
		delete this->transform;
		//std::cout << "End GameObject Destruction..." << std::endl;
	}

	void GameObject::Init() 
	{
		for (K::Component* c : this->components)
		{
			//std::cout << "Initializing " << c->GetName() << std::endl;
			c->Init();
		}
	}

	void GameObject::RenderInit()
	{
		for (K::Component* c : this->components)
		{
			//std::cout << "Initializing " << c->GetName() << std::endl;
			c->RenderInit();
		}
	}

	void GameObject::Update()
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Update();
		}
	}

	void GameObject::RenderBind() 
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->RenderBind();
		}
	}

	void GameObject::Render() 
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Render();
		}
	}

	void GameObject::RenderUnbind() 
	{
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->RenderUnbind();
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

		if (this->GetMaterial() != nullptr) 
		{
			glUseProgram(this->GetMaterial()->GetShader()->shader);

			glUniformMatrix4fv(this->GetMaterial()->GetUniform("modelMatrix"), 1, GL_FALSE, &this->GetTransform()->modelMatrix.m[0][0]);
		}
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