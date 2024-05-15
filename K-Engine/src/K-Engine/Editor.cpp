#include "Editor.h"
#include "Serializer.h"

namespace K 
{
	K::Material* K::Editor::material;
	K::GameObject* K::Editor::selectedGameObject;
	K::SceneManager* K::Editor::sceneManager;
	std::vector<K::GameObject*> K::Editor::deleteArray;
	std::map<std::string, IFactory*> K::Editor::lst{ {typeid(K::Sprite).name() , new K::Factory<K::Sprite>} , {typeid(K::Player).name() , new K::Factory<K::Player>} ,{typeid(K::Mesh).name() , new K::Factory<K::Mesh>} ,{typeid(K::Camera).name() , new K::Factory<K::Camera>} ,{typeid(K::Collider).name() , new K::Factory<K::Collider>} ,{typeid(K::Animator).name() , new K::Factory<K::Animator>}, {typeid(K::Ship).name() , new K::Factory<K::Ship>}, {typeid(K::Timer).name() , new K::Factory<K::Timer>}, {typeid(K::Move).name() , new K::Factory<K::Move>}, {typeid(K::TriggerDeath).name() , new K::Factory<K::TriggerDeath>} , {typeid(K::Light).name() , new K::Factory<K::Light>}, {typeid(K::FollowPlayer).name() , new K::Factory<K::FollowPlayer>} };

	Editor::Editor(K::Window* window, K::SceneManager* sceneManager, K::Material* material)
	{
		//IMGUI Setup Stuffs
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		ImGui::StyleColorsLight();
		this->sceneManager = sceneManager;
		this->window = window;
		this->material = material;
		this->buildWindow = false;
		this->saveWindow = false;
		ImGui_ImplGlfw_InitForOpenGL(this->window->window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	Editor::~Editor()
	{
		delete this->material;
		delete this->window;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	K::Scene* Editor::GetCurrentScene() 
	{
		return K::SceneManager::currentScene;
	}

	K::GameObject* Editor::GetSelectedGameObject() 
	{
		return selectedGameObject;
	}

	K::Material* Editor::GetMaterial() 
	{
		return K::Editor::material;
	}

	void Editor::ImGuiBegin()
	{
		//IMGUI Panel Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) 
		{
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		}
	}

	void Editor::ImGuiEnd() 
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Editor::ImGuiViewport() 
	{
		if (ImGui::Begin("Viewport")) 
		{
			
		}
		ImGui::End();
	}

	void Editor::ImGuiInspector() 
	{
		if (ImGui::Begin("K-Engine Properties"))
		{
			/*if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene"))
					{
						K::Editor::GetCurrentScene()->CreateEmptyScene();
						this->selectedGameObject = nullptr;
					}
					if (ImGui::MenuItem("Open..."))
					{
						file = ImGui::FileBrowser();
						file.SetTitle("Load Scene");
						file.SetTypeFilters({ ".JAWS" });
						file.SetPwd(ASSET_DIR);
						file.Open();
					}
					if (ImGui::MenuItem("Save..."))
					{
						K::Serializer serialize = K::Serializer(K::Editor::GetCurrentScene(), ASSET_DIR + K::Editor::GetCurrentScene()->GetLocation());
					}
					if (ImGui::MenuItem("Save Scene..."))
					{
						file = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
						file.SetTitle("Save Scene");
						file.SetTypeFilters({ ".JAWS" });
						file.SetPwd(ASSET_DIR);
						file.Open();
						this->saveWindow = true;
					}
					if (ImGui::MenuItem("Build..."))
					{
						this->buildWindow = true;
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
			if (this->buildWindow)
			{
				if (ImGui::BeginChild("Build Menu"))
				{
					if (ImGui::Button("Add Scene"))
					{
						file = ImGui::FileBrowser();
						file.SetTitle("Load Scene");
						file.SetTypeFilters({ ".JAWS" });
						file.SetPwd(ASSET_DIR);
						file.Open();
					}
					if (file.HasSelected())
					{
						std::string location = file.GetSelected().string();
						std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
						this->sceneManager->AddScene(relativeLocation);
						file.ClearSelected();
					}
					if (ImGui::BeginListBox("Scenes"))
					{
						for (int i = 0; i < this->sceneManager->GetNumberOfScenes(); i++)
						{
							if (ImGui::Selectable(this->sceneManager->GetSceneName(i)))
							{
								this->selectedScene = i;
							}
						}
						ImGui::EndListBox();
					}

					if (this->selectedScene >= 0)
					{
						ImGui::Text(this->sceneManager->GetSceneName(this->selectedScene));
					}

					if (ImGui::Button("Save Build Menu"))
					{
						this->sceneManager->SaveSceneManager();
						this->buildWindow = false;
					}

					ImGui::SameLine();

					if (ImGui::Button("Delete Scene From Build"))
					{
						this->sceneManager->DeleteScene(this->selectedScene);
						this->selectedScene = -1;
					}

					if (ImGui::Button("Exit Build Menu"))
					{
						this->buildWindow = false;
					}
				}
				ImGui::EndChild();
			}
			else if (this->saveWindow)
			{
				if (file.HasSelected())
				{
					std::string location = file.GetSelected().string();
					if (location.find(".JAWS") == std::string::npos)
					{
						location += ".JAWS";
					}
					K::Serializer serialize = K::Serializer(K::Editor::GetCurrentScene(), location);
					file.ClearSelected();
					this->saveWindow = false;
				}
			}
			else
			{
				if (file.HasSelected())
				{
					K::Editor::GetCurrentScene()->CreateEmptyScene();
					this->selectedGameObject = nullptr;
					std::string location = file.GetSelected().string();
					std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
					K::Deserializer deserialize = K::Deserializer(K::Editor::GetCurrentScene(), relativeLocation);
					file.ClearSelected();
				}
			}*/

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (ImGui::GetIO().Framerate), (ImGui::GetIO().Framerate));

			ImGui::Text("Number of GameObjects: %i", K::Editor::GetCurrentScene()->GetNumberOfObjects());

			ImGuiExtra();
		}
		ImGui::End();
	}

	bool Editor::Render() 
	{
		ImGuiBegin();

		ImGuiInspector();

		ImGuiHierarchy();

		ImGuiViewport();

		ImGuiEnd();

		return false;
	}

	void Editor::Delete(K::GameObject* target) 
	{
		K::Editor::deleteArray.push_back(target);
		if (target == K::Editor::selectedGameObject) 
		{
			K::Editor::selectedGameObject = nullptr;
		}
	}

	void Editor::ShowChildren(K::GameObject* current) 
	{
		for (int i : current->children) 
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			if (K::Editor::GetCurrentScene()->GetGameObjects().at(i)->children.size() <= 0)
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
			}
			bool nodeOpen = ImGui::TreeNodeEx(K::Editor::GetCurrentScene()->GetGameObjects().at(i)->GetName(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				this->selectedGameObject = K::Editor::GetCurrentScene()->GetGameObjects().at(i);
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("_CHILD", &i, sizeof(int));
				ImGui::Text(K::Editor::GetCurrentScene()->GetGameObjects().at(i)->GetName());
				ImGui::EndDragDropSource();
			}
			else if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_CHILD"))
				{
					int child = *(const int*)payload->Data;
					K::Editor::GetCurrentScene()->GetGameObjects().at(child)->SetParent(K::Editor::GetCurrentScene()->GetGameObjects().at(i));
				}
				ImGui::EndDragDropTarget();
			}

			if (nodeOpen)
			{
				if (K::Editor::GetCurrentScene()->GetGameObjects().at(i)->children.size() > 0)
				{
					this->ShowChildren(K::Editor::GetCurrentScene()->GetGameObjects().at(i));
				}
				ImGui::TreePop();
			}
		}
	}

	void Editor::ParentChildrenHierarchy() 
	{
		bool sceneOpen = ImGui::TreeNodeEx(K::Editor::GetCurrentScene()->GetSceneName(), ImGuiTreeNodeFlags_DefaultOpen);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_CHILD"))
			{
				int child = *(const int*)payload->Data;
				K::Editor::GetCurrentScene()->GetGameObjects()[child]->SetParent(nullptr);
			}
			ImGui::EndDragDropTarget();
		}
		if (sceneOpen) 
		{
			for (auto temp : K::Editor::GetCurrentScene()->GetGameObjects())
			{
				if (temp.second->parent == nullptr)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					if (temp.second->children.size() <= 0)
					{
						flags |= ImGuiTreeNodeFlags_Leaf;
					}
					bool nodeOpen = ImGui::TreeNodeEx(temp.second->GetName(), flags);
					if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
					{
						this->selectedGameObject = temp.second;
					}
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("_CHILD", &temp.first, sizeof(int));
						ImGui::Text(temp.second->GetName());
						ImGui::EndDragDropSource();
					}
					else if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_CHILD"))
						{
							int child = *(const int*)payload->Data;
							K::Editor::GetCurrentScene()->GetGameObjects().at(child)->SetParent(temp.second);
						}
						ImGui::EndDragDropTarget();
					}
					if (nodeOpen)
					{
						if (temp.second->children.size() > 0)
						{
							this->ShowChildren(temp.second);
						}
						ImGui::TreePop();
					}
				}
			}
			ImGui::TreePop();
		}
	}

	void Editor::ImGuiHierarchy()
	{
		std::string temp = "Hierarchy: ";
		temp += K::Editor::GetCurrentScene()->GetSceneName();
		ImGui::Begin(temp.c_str());

		this->ParentChildrenHierarchy();

		ImGui::End();
	}

	void Editor::ImGuiExtra() 
	{
		if (ImGui::Button("Create new GameObject")) 
		{
			K::GameObject* temp = new K::GameObject("Name", new K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f)));
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
			if (ImGui::Button("Duplicate")) 
			{
				K::GameObject* temp = new K::GameObject("Name", new K::Transform(new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f)));
				*temp = *this->selectedGameObject;
				std::string name = temp->GetName();
				bool changedName = false;
				for (int i = name.length(); i > 0; i--) 
				{
					if (std::isdigit(name[i])) 
					{
						std::string temp{ name[i] };
						int value = std::stoi(temp);
						int newValue = value + 1;
						name.erase(i);
						name += std::to_string(newValue);
						changedName = true;
						break;
					}
				}
				if (changedName) 
				{
					temp->SetName(name);
				}
				else 
				{
					name += std::to_string(0);
					temp->SetName(name);
				}
				this->selectedGameObject = temp;
			}
			if (ImGui::BeginPopup("Components"))
			{
				for(auto const& x : this->lst)
				{
					if (ImGui::MenuItem(x.first.c_str()))
					{
						K::Component* temp = x.second->create();
						this->selectedGameObject->AddComponent(temp);
						temp->Init();
						break;
					}
				}
				ImGui::EndPopup();
			}
		}
	}
}