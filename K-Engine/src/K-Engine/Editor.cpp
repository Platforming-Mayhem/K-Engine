#include "Editor.h"
#include "Serializer.h"

namespace K 
{
	K::GameObject* K::Editor::selectedGameObject;
	K::Scene* K::Editor::currentScene;

	Editor::Editor(K::Window* window, K::Scene* scene, K::Material* material)
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
		this->material = material;
	}

	Editor::~Editor()
	{
		delete this->currentScene;
		delete this->material;
		delete this->window;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	K::Scene* Editor::GetScene() 
	{
		return currentScene;
	}

	K::GameObject* Editor::GetSelectedGameObject() 
	{
		return selectedGameObject;
	}

	K::Material* Editor::GetMaterial() 
	{
		return this->material;
	}

	bool Editor::Render() 
	{
		//IMGUI Panel Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("K-Engine Properties", NULL, ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene"))
					{
						this->currentScene->CreateEmptyScene();
						this->selectedGameObject = nullptr;
					}
					if (ImGui::MenuItem("Open..."))
					{
						file.SetTitle("Load Scene");
						file.SetTypeFilters({ ".JAWS" });
						file.Open();
					}
					if (ImGui::MenuItem("Save..."))
					{
						K::Serializer serialize = K::Serializer(this->currentScene);
					}
					if (ImGui::MenuItem("Exit"))
					{
						return true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			file.Display();
			if (file.HasSelected())
			{
				this->currentScene->CreateEmptyScene();
				this->selectedGameObject = nullptr;
				std::string location = file.GetSelected().string();
				K::Deserializer deserialize = K::Deserializer(this->currentScene, location, this);
				file.ClearSelected();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (ImGui::GetIO().Framerate), (ImGui::GetIO().Framerate));

			ImGui::Text("Number of GameObjects: %i", this->currentScene->GetNumberOfObjects());

			ImGuiExtra();

			ImGui::End();

			ImGuiHierarchy();

			ImGui::Render();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			return false;
		}
	}

	void Editor::Delete(K::GameObject* temp) 
	{
		K::Editor::currentScene->Delete(temp);
		K::Editor::selectedGameObject = nullptr;
	}

	void Editor::ImGuiHierarchy()
	{
		ImGui::Begin("Hierarchy");

		for (int i = 0; i < this->currentScene->GetNumberOfObjects(); i++) 
		{
			if (ImGui::Selectable(this->currentScene->GetGameObjects()[i]->GetName()))
			{
				this->selectedGameObject = this->currentScene->GetGameObjects()[i];
			}
		}

		ImGui::End();
	}

	void Editor::ImGuiExtra() 
	{
		if (ImGui::Button("Create new GameObject")) 
		{
			K::GameObject* temp = new K::GameObject("Name", new K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f)), this->material);
			this->currentScene->Attach(temp);
			this->selectedGameObject = temp;
		}
		if (this->selectedGameObject != NULL)
		{
			this->selectedGameObject->UpdateEditor();

			if (ImGui::Button("Delete")) 
			{
				K::Editor::Delete(this->selectedGameObject);
			}
			ImGui::SameLine();
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				for(int i = 0; i < this->lst.size(); i++)
				{
					K::Component* temp = this->lst[i]->create();
					if (ImGui::MenuItem(temp->GetName()))
					{
						this->selectedGameObject->AddComponent(temp);
						temp->Init();
						break;
					}
					delete temp;
				}
				ImGui::EndPopup();
			}
		}
	}
}