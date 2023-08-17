#include "GameObject.h"
#include "Editor.h"
#include "InputManager.h"

namespace K 
{
	GameObject::GameObject(const char* name, Transform* transform, K::Shader* shader)
	{
		this->shader = shader;
		this->name = (char*)name;
		this->transform = transform;
		this->mesh = new Mesh();

		glEnable(GL_CULL_FACE);
		glCreateVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		this->Bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(int), &this->mesh->indices[0], GL_DYNAMIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(K::Vertex), &this->mesh->vertices[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		this->Unbind();
	}

	GameObject::~GameObject() 
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
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
			if (ImGui::Button("Load Model"))
			{
				file.SetTitle("Load Model");
				file.SetTypeFilters({ ".FBX", ".OBJ"});
				file.Open();
			}
			file.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				if (this->mesh->LoadModelsAssimp(location))
				{
					std::cout << "Found model" << std::endl;
					this->meshUpdate = true;
				}
				file.ClearSelected();
			}
		}
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->UpdateEditor();
		}
	}

	void GameObject::PassTransformationMatrix()
	{
		K::GameObject::GetTransform()->PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->shader->shader, "modelMatrix"), 1, GL_FALSE, &K::GameObject::GetTransform()->modelMatrix.m[0][0]);
	}

	void GameObject::Bind() 
	{
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(int), &this->mesh->indices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(K::Vertex), &this->mesh->vertices[0], GL_DYNAMIC_DRAW);
		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Bind();
		}
		PassTransformationMatrix();
	}

	void GameObject::Unbind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		for (int i = 0; i < this->GetNumberOfComponents(); i++)
		{
			this->GetComponent(i)->Unbind();
		}
	}

	void GameObject::Render() 
	{
		this->Bind();

		glDrawElements(GL_TRIANGLES, this->mesh->indices.size() * sizeof(int), GL_UNSIGNED_INT, nullptr);

		this->Unbind();
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