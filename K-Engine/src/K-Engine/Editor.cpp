#include "Editor.h"

namespace K 
{
	Editor::Editor(K::Window* window, K::Scene* scene, K::Shader* shader)
	{
		//IMGUI Setup Stuffs
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window->window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		this->currentScene = scene;
		this->window = window;
		this->shader = shader;
		this->lst.push_back(new K::Factory<K::Sprite>);
		this->lst.push_back(new K::Factory<K::Player>);
		this->lst.push_back(new K::Factory<K::Collider>);
	}

	Editor::~Editor()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	K::GameObject* Editor::GetSelectedGameObject() 
	{
		return selectedGameObject;
	}

	K::GameObject* K::Editor::selectedGameObject;

	void Editor::Render() 
	{
		//IMGUI Panel Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("K-Engine Properties");

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (ImGui::GetIO().Framerate), (ImGui::GetIO().Framerate));

			ImGui::Text("Number of GameObjects: %i", this->currentScene->GetNumberOfObjects());

			ImGuiExtra();

			ImGui::End();

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void Editor::ImGuiExtra() 
	{
		if (ImGui::Button("Locate Camera")) 
		{
			this->selectedGameObject = this->currentScene->GetGameObjects()[0];
		}
		if (ImGui::Button("Create new GameObject")) 
		{
			K::GameObject* temp = new K::GameObject("Name", new K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f)), this->shader);
			this->currentScene->Attach(temp);
			this->selectedGameObject = temp;
		}
		for (int i = 0; i < this->currentScene->GetNumberOfObjects(); i++)
		{
			if (this->currentScene->GetGameObjects()[i]->IsSelected(this->window))
			{
				this->selectedGameObject = this->currentScene->GetGameObjects()[i];
			}
		}
		if (this->selectedGameObject != NULL)
		{
			this->selectedGameObject->UpdateEditor();
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				for(K::IFactory* comp : this->lst)
				{
					K::Component* temp = comp->create();
					if (ImGui::MenuItem(temp->GetName()))
					{
						this->selectedGameObject->AddComponent(temp);
					}
				}
				ImGui::EndPopup();
			}
		}
	}
}