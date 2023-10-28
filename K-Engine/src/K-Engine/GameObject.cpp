#include "GameObject.h"
#include "Editor.h"
#include "InputManager.h"

namespace K 
{
	GameObject::GameObject(const char* name, Transform* transform)
	{
		this->name = (char*)name;
		this->transform = transform;
	}

	GameObject::~GameObject() 
	{
		
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
		ImGui::InputText("Name:", &this->name);
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

	void GameObject::PassTransformationMatrix(K::Material* mat)
	{
		K::GameObject::GetTransform()->PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(mat->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &K::GameObject::GetTransform()->modelMatrix.m[0][0]);
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

	bool GameObject::IsSelected(K::Window* window)
	{
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow))
		{
			return false;
		}
		else 
		{
			double x, y;
			int width, height;
			glfwGetWindowSize(window->window, &width, &height);
			glfwGetCursorPos(window->window, &x, &y);
			K::Vector3 displacement = K::Vector3(-InputManager::GetMousePosition(window).x, InputManager::GetMousePosition(window).y, 0.0f);
			displacement -= *this->transform->position;
			displacement = K::Vector3(displacement.x, displacement.y, 0.0f);
			if (displacement.magnitude() < 1.0f && glfwGetMouseButton(window->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}