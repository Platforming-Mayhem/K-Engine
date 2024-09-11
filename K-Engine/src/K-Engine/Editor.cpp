#include "Editor.h"
#include <K_Engine.h>

namespace K 
{
	float K::Editor::windowScaleFactor = 1.0f;
	float K::Editor::offsetX = 0.0f;
	float K::Editor::offsetY = 0.0f;

	K::GameObject* K::Editor::selectedGameObject;
	K::SceneManager* K::Editor::sceneManager;
	std::vector<K::GameObject*> K::Editor::deleteArray;
	std::map<std::string, IFactory*> K::Editor::lst{ {typeid(K::Sprite).name() , new K::Factory<K::Sprite>} , {typeid(K::Player).name() , new K::Factory<K::Player>} ,{typeid(K::Mesh).name() , new K::Factory<K::Mesh>} ,{typeid(K::Camera).name() , new K::Factory<K::Camera>} ,{typeid(K::Collider).name() , new K::Factory<K::Collider>} ,{typeid(K::Animator).name() , new K::Factory<K::Animator>}, {typeid(K::Timer).name() , new K::Factory<K::Timer>}, {typeid(K::Move).name() , new K::Factory<K::Move>}, {typeid(K::TriggerDeath).name() , new K::Factory<K::TriggerDeath>} , {typeid(K::Light).name() , new K::Factory<K::Light>}, {typeid(K::Shooter).name() , new K::Factory<K::Shooter>}, {typeid(K::Crush).name() , new K::Factory<K::Crush>}, {typeid(K::TriggerNextScene).name() , new K::Factory<K::TriggerNextScene>}, {typeid(K::ButtonLoadScene).name() , new K::Factory<K::ButtonLoadScene>} };

	Editor::Editor(K::Window* window, K::SceneManager* sceneManager)
	{
		//IMGUI Setup Stuffs
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Viewport
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		ImGui::StyleColorsDark();
		this->sceneManager = sceneManager;
		this->window = window;
		this->buildWindow = false;
		this->viewport = new K::RenderTexture(this->window->width, this->window->height, GL_TEXTURE_2D);
		this->currentDirectory = ASSET_DIR;
		ImGui_ImplGlfw_InitForOpenGL(this->window->window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		#if _DEBUG
		this->AddPreloadedTexture("textures/editor/scene.png");
		this->AddPreloadedTexture("textures/editor/unknown.png");
		this->AddPreloadedTexture("textures/editor/file.png");
		#endif
	}

	Editor::~Editor()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		for (auto tex : this->preloadedTextures)
		{
			delete tex.second;
		}
		this->preloadedTextures.clear();

		delete this->window;
		delete this->viewport;
	}

	K::RenderTexture* Editor::GetViewport() 
	{
		return this->viewport;
	}

	K::Scene* Editor::GetCurrentScene() 
	{
		return K::SceneManager::currentScene;
	}

	K::GameObject* Editor::GetSelectedGameObject() 
	{
		return selectedGameObject;
	}

	void Editor::ImGuiBegin()
	{
		//IMGUI Panel Stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) 
		{
			ImGui::DockSpaceOverViewport();
		}
	}

	void Editor::ImGuiEnd() 
	{
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Editor::ImGuiViewport() 
	{
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&window_class);
		if (ImGui::Begin("Viewport"))
		{
			int height = ImGui::GetWindowHeight() - ImGui::GetFrameHeight() - 2.0f;
			int width = height * (16.0f / 9.0f);
			K::Editor::windowScaleFactor = (float)height / (float)this->viewport->GetHeight();
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - width) * 0.5f, (ImGui::GetWindowHeight() - height) * 0.5f));
			ImGui::Image((void*)(intptr_t)this->viewport->GetID(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));
		}
		K::Editor::offsetX = ImGui::GetItemRectMin().x;
		K::Editor::offsetY = ImGui::GetItemRectMin().y;
		if (glfwGetMouseButton(this->window->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && ImGui::IsItemHovered())
		{
			for (auto temp : this->sceneManager->currentScene->GetGameObjects())
			{
				if (temp.second->GetComponentOfType(typeid(K::Camera).name()) != nullptr)
				{
					K::Camera* camera = (K::Camera*)temp.second->GetComponentOfType(typeid(K::Camera).name());
					K::Editor::selectedGameObject = InputManager::PickGameObject(camera);
				}
			}
		}
		ImGui::End();
	}

	void Editor::ImGuiInspector() 
	{
		if (ImGui::Begin("K-Engine Properties"))
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Scene"))
					{
						K::Editor::GetCurrentScene()->CreateEmptyScene();
						this->selectedGameObject = nullptr;
					}
					if (ImGui::MenuItem("Save..."))
					{
						this->sceneName = K::Editor::GetCurrentScene()->GetSceneName();
						this->confirmationWindow = true;
					}
					if (ImGui::MenuItem("Build..."))
					{
						this->buildWindow = true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			if (this->confirmationWindow) 
			{
				ImGui::OpenPopup("Confirmation Window");
				this->confirmationWindow = false;
			}
			if (ImGui::BeginPopup("Confirmation Window")) 
			{
				ImGui::InputText("Scene Name: ", &this->sceneName);
				ImGui::Text("Are you sure you want to overwrite this scene?");
				if (ImGui::Button("Yes")) 
				{
					if(K::Editor::GetCurrentScene()->GetSceneName() != this->sceneName)
						K::Editor::GetCurrentScene()->RenameScene(this->sceneName);
					K::Serializer serialize = K::Serializer(K::Editor::GetCurrentScene(), ASSET_DIR + K::Editor::GetCurrentScene()->GetLocation());
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("No")) 
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (this->buildWindow)
			{
				if (ImGui::BeginListBox("Scenes"))
				{
					for (int i = 0; i < this->sceneManager->GetNumberOfScenes(); i++)
					{
						if (ImGui::Selectable(this->sceneManager->GetSceneName(i)))
						{
							this->selectedScene = i;
						}
					}
					ImGui::Dummy(ImGui::GetContentRegionAvail());
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENE"))
						{
							const char* file = (const char*)payload->Data;
							this->sceneManager->AddScene(file);
						}
						ImGui::EndDragDropTarget();
					}
				}
				ImGui::EndListBox();

				if (this->selectedScene >= 0)
				{
					ImGui::Text(this->sceneManager->GetSceneName(this->selectedScene));
				}

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

				if (ImGui::Button("Save Build Menu"))
				{
					this->sceneManager->SaveSceneManager();
					this->buildWindow = false;
				}

				ImGui::PopStyleColor();

				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

				if (ImGui::Button("Delete Scene From Build"))
				{
					this->sceneManager->DeleteScene(this->selectedScene);
					this->selectedScene = -1;
				}

				ImGui::PopStyleColor();

				if (ImGui::Button("Exit Build Menu"))
				{
					this->buildWindow = false;
				}
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (ImGui::GetIO().Framerate), (ImGui::GetIO().Framerate));

			ImGui::Text("Number of GameObjects: %i", K::Editor::GetCurrentScene()->GetNumberOfObjects());

			ImGuiExtra();
		}
		ImGui::End();
	}

	void Editor::AddPreloadedTexture(std::string location) 
	{
		this->preloadedTextures.insert({ location, new K::Texture(location.c_str()) });
	}

	void Editor::LoadPreloadedTextures() 
	{
		for (auto temp : this->preloadedTextures) 
		{
			temp.second->LoadIntoGPU();
		}
	}

	void Editor::ImGuiContentBrowser() 
	{
		this->LoadPreloadedTextures();

		ImGui::Begin("K-Engine Content Browser");

		if (this->currentDirectory.string() != ASSET_DIR && this->currentDirectory.string() + "/" != ASSET_DIR)
		{
			if (ImGui::Button("...")) 
			{
				this->currentDirectory = this->currentDirectory.parent_path();
			}
		}

		float contentWidth = ImGui::GetContentRegionAvail().x;
		int numberOfColumns = contentWidth / 256.0f;
		ImGui::Columns(numberOfColumns, "Content Columns", false);

		for (auto& p : std::filesystem::directory_iterator(this->currentDirectory)) 
		{
			std::string relativeLocation = std::filesystem::relative(p.path(), ASSET_DIR).string();
			if (p.is_directory()) 
			{
				K::Texture* temp = this->preloadedTextures.at("textures/editor/file.png");
				if (ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)temp->GetViewID(), ImVec2(temp->GetWidth(), temp->GetHeight()), ImVec2(0, 1), ImVec2(1, 0)))
				{
					this->currentDirectory = p.path();
				}
				ImGui::TextWrapped(relativeLocation.c_str());
				ImGui::NextColumn();
			}
			else 
			{
				if (p.is_regular_file())
				{
					numberOfColumns = contentWidth / 128.0f;
					if (p.path().extension() == ".png")
					{
						if (this->preloadedTextures.contains(relativeLocation)) 
						{
							K::Texture* temp = this->preloadedTextures.at(relativeLocation);
							ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)temp->GetID(), ImVec2(128.0f, 128.0f), ImVec2(0, 1), ImVec2(1, 0));
							if (ImGui::BeginDragDropSource())
							{
								std::string file = temp->GetFilePath();
								ImGui::SetDragDropPayload("_TEXTURE", file.c_str(), file.size() + 1);
								ImGui::Text(file.c_str());
								ImGui::EndDragDropSource();
							}
						}
						else 
						{
							this->AddPreloadedTexture(relativeLocation);
						}
					}
					else if (p.path().extension() == ".gif") 
					{
						if (this->preloadedTextures.contains(relativeLocation))
						{
							K::Texture* temp = this->preloadedTextures.at(relativeLocation);
							ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)temp->GetID(), ImVec2(128.0f, 128.0f), ImVec2(0, 1), ImVec2(1, 0));
							if (ImGui::BeginDragDropSource())
							{
								std::string file = temp->GetFilePath();
								ImGui::SetDragDropPayload("_TEXTURE", file.c_str(), file.size() + 1);
								ImGui::Text(file.c_str());
								ImGui::EndDragDropSource();
							}
						}
						else
						{
							this->AddPreloadedTexture(relativeLocation);
						}
					}
					else if (p.path().extension() == ".JAWS")
					{
						K::Texture* scene = this->preloadedTextures.at("textures/editor/scene.png");
						if (ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)scene->GetID(), ImVec2(scene->GetWidth(), scene->GetWidth()), ImVec2(0, 1), ImVec2(1, 0)))
						{
							K::Editor::GetCurrentScene()->CreateEmptyScene();
							this->selectedGameObject = nullptr;
							K::Deserializer deserialize = K::Deserializer(K::SceneManager::currentScene, relativeLocation);
						}
						if (ImGui::BeginDragDropSource())
						{
							std::string file = relativeLocation;
							ImGui::SetDragDropPayload("_SCENE", file.c_str(), file.size() + 1);
							ImGui::Text(file.c_str());
							ImGui::EndDragDropSource();
						}
					}
					else 
					{
						K::Texture* unknown = this->preloadedTextures.at("textures/editor/unknown.png");
						ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)unknown->GetID(), ImVec2(unknown->GetWidth(), unknown->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
					}
				}
				ImGui::TextWrapped(std::filesystem::relative(p.path(), ASSET_DIR).filename().string().c_str());
				ImGui::NextColumn();
			}
		}

		ImGui::End();
	}

	bool Editor::Render() 
	{
		ImGuiBegin();

		ImGuiInspector();

		ImGuiHierarchy();

		ImGuiContentBrowser();

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
			this->GetViewport()->Bind();
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
						temp->RenderInit();
						break;
					}
				}
				ImGui::EndPopup();
			}
			this->GetViewport()->Unbind();
		}
	}
}