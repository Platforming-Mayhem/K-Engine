#include "K-Engine/Editor.h"
#include <K_Engine.h>

std::string ASSET_DIR = std::filesystem::current_path().parent_path().parent_path().string() + "/assets/";

namespace K 
{
	float K::Editor::windowScaleFactor = 1.0f;
	float K::Editor::offsetX = 0.0f;
	float K::Editor::offsetY = 0.0f;
	std::filesystem::path K::Editor::currentDirectory;

	K::Matrix4x4* K::Editor::projectionMatrix;

	K::Matrix4x4* K::Editor::viewMatrix;

	K::Vector3* K::Editor::cameraPosition;

	K::GameObject* K::Editor::selectedGameObject;

	K::SceneManager* K::Editor::sceneManager;

	std::vector<K::GameObject*> K::Editor::deleteArray;

	std::unordered_map<std::string, IFactory*>& K::Editor::lst() 
	{
		static std::unordered_map<std::string, IFactory*> temp;
		return temp;
	}

	Editor::Editor(K::Window* window, K::SceneManager* sceneManager)
	{
		this->sceneManager = sceneManager;
		this->window = window;
		this->buildWindow = false;

		std::string currentDir = std::filesystem::current_path().string();
		currentDir += "/assets/";
		if (std::filesystem::is_directory(currentDir))
		{
			ASSET_DIR = currentDir;
		}

		K::Editor::SetDirectory(ASSET_DIR);

		K::Editor::sceneManager->LoadBuildMenu("buildScenes.txt");

		this->viewport = new K::RenderTexture(this->window->width, this->window->height, GL_TEXTURE_2D);

		#if _DEBUG
		//IMGUI Setup Stuffs
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Viewport
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		ImGui::StyleColorsDark();

		if (std::ifstream(io.IniFilename).good())
			this->doesIniExist = true;
		else
			this->doesIniExist = false;

		ImGui_ImplGlfw_InitForOpenGL(this->window->window, true);
		ImGui_ImplOpenGL3_Init("#version 330 compatibility");
		this->AddPreloadedTexture("textures/editor/scene.png");
		this->AddPreloadedTexture("textures/editor/unknown.png");
		this->AddPreloadedTexture("textures/editor/file.png");

		int width, height, c;
		unsigned char* icon = stbi_load((ASSET_DIR + "textures/watermark/watermark.png").c_str(), &width, &height, &c, 0);

		GLFWimage glfwImage;
		glfwImage.width = width;
		glfwImage.height = height;
		glfwImage.pixels = icon;

		glfwSetWindowIcon(this->window->window, 1, &glfwImage);

		stbi_image_free(icon);

		#else
		this->LoadComponents();
		#endif
	}

	Editor::~Editor()
	{
		this->UnloadComponents();

		#if _DEBUG
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		for (auto tex : this->preloadedTextures)
		{
			delete tex.second;
		}
		this->preloadedTextures.clear();
		#endif

		delete this->viewport;
		delete this->window;
	}

	void Editor::UnloadComponents() 
	{
		if (this->componentsLibrary != NULL) 
		{
			std::cout << "Unloading components..." << std::endl;
			K::Editor::lst().clear();
			#ifdef _WIN32
			FreeLibrary(this->componentsLibrary);
			#elif __unix__
			dlclose(this->componentsLibrary);
			#endif
			std::cout << "Unloaded components" << std::endl;
		}
	}

	void Editor::LoadComponents() 
	{
		this->UnloadComponents();
		#ifdef _WIN32
		this->componentsLibrary = LoadLibrary(TEXT("Components"));

		if (this->componentsLibrary != NULL)
		{
			std::cout << "Found Library" << std::endl;
		}
		else
		{
			std::cout << "No Library Found" << std::endl;
		}

		#elif __unix__
		this->componentsLibrary = dlopen("./libComponents.so", RTLD_NOW);
		if (this->componentsLibrary)
		{
			std::cout << "Found Library" << std::endl;
		}
		else
		{
			std::cout << "No Library Found" << std::endl;
		}
		#endif
	}

	ImGuiContext* Editor::GetImGuiContext() 
	{
		return ImGui::GetCurrentContext();
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
			ImGuiID dockspace = ImGui::DockSpaceOverViewport();
			if (!this->doesIniExist) 
			{
				ImGuiID top, bottom, right, left;
				ImGui::DockBuilderRemoveNode(dockspace);
				ImGui::DockBuilderAddNode(dockspace);
				ImGui::DockBuilderSplitNode(dockspace, ImGuiDir_Right, 0.8f, &right, &left);
				ImGui::DockBuilderSplitNode(right, ImGuiDir_Up, 0.6f, &top, &bottom);
				ImGui::DockBuilderDockWindow("Viewport", top);
				ImGui::DockBuilderDockWindow("K-Engine Content Browser", bottom);
				ImGui::DockBuilderSplitNode(left, ImGuiDir_Up, 0.5f, &top, &bottom);
				ImGui::DockBuilderDockWindow("K-Engine Properties", top);
				ImGui::DockBuilderDockWindow("###Hierarchy", bottom);
				ImGui::DockBuilderFinish(dockspace);
				std::cout << "Create Default Layout" << std::endl;
				this->doesIniExist = true;
			}
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
			K::Editor::selectedGameObject = InputManager::PickGameObject();
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
					if (ImGui::MenuItem("Create/Open Project"))
					{
						this->projectLoadWindow = true;
					}
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
			if (this->projectLoadWindow) 
			{
				char* location;
				std::string projectName;

				if (NFD_Init()) 
				{
					if (NFD_PickFolderU8(&location, NULL) == NFD_OKAY)
					{
						ASSET_DIR = location;
						ASSET_DIR += "/assets/";
						if (!std::filesystem::is_directory(ASSET_DIR))
						{
							std::filesystem::create_directory(ASSET_DIR);
						}
						K::Editor::SetDirectory(ASSET_DIR);
						projectName = location;
						NFD_FreePathU8(location);
					}
					NFD_Quit();
				}

				this->updateFiles = true;

				this->projectPath = projectName;
				projectName = std::filesystem::absolute(projectName).filename().string();

				glfwSetWindowTitle(K::window->window, projectName.c_str());

				std::string cmakelist = ASSET_DIR + "CMakeLists.txt";

				if (!std::filesystem::exists(cmakelist) || !std::filesystem::exists(ASSET_DIR + "bin"))
				{
					FILE* file = fopen(cmakelist.c_str(), "w");

					fputs("cmake_minimum_required(VERSION 3.8) \n"
						"set(CMAKE_CXX_STANDARD 23) \n"
						"project(Components) \n"
						"IF(UNIX) \n"
						"ADD_DEFINITIONS(-D_DEBUG) \n"
						"ENDIF() \n", file);

					std::string path = std::filesystem::current_path().parent_path().string() + "/Debug";

					std::replace(path.begin(), path.end(), '\\', '/');

					std::string cmakeCommand = std::format("set(CMAKE_LIBRARY_OUTPUT_DIRECTORY {0}) \n", path);

					fputs(cmakeCommand.c_str(), file);

					cmakeCommand = std::format("set(CMAKE_RUNTIME_OUTPUT_DIRECTORY {0}) \n", path);

					fputs(cmakeCommand.c_str(), file);

					path = std::filesystem::current_path().parent_path().parent_path().string();

					std::replace(path.begin(), path.end(), '\\', '/');

					cmakeCommand = std::format("add_subdirectory(\"{0}\" \"{0}/bin\") \n", path);

					fputs(cmakeCommand.c_str(), file);

					fputs("file(GLOB ComponentSrcs CONFIGURE_DEPENDS \"components/*.cpp\") \n"
						"add_library(${PROJECT_NAME} SHARED ${ComponentSrcs}) \n"
						"target_compile_definitions(${PROJECT_NAME} PRIVATE KC_BUILD_DLL) \n"
						"target_link_libraries(${PROJECT_NAME} K-Engine) \n"
						"IF (MSVC) \n"
						"set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY \"$(ProjectDir) / $(Configuration)\") \n"
						"ENDIF()", file);

					fclose(file);

					cmakelist = std::format("cmake -B \"{0}\" -S \"{1}\"", ASSET_DIR + "bin/Debug", ASSET_DIR.substr(0, ASSET_DIR.size() - 1));

					std::cout << cmakelist << std::endl;

					std::system(cmakelist.c_str());
				}

				K::Editor::sceneManager->LoadBuildMenu("buildScenes.txt");

				this->msvcBuildWindow = true;

				this->projectLoadWindow = false;
			}
			if (this->confirmationWindow) 
			{
				ImGui::OpenPopup("Confirmation Window");
				this->confirmationWindow = false;
			}
			if (this->msvcBuildWindow)
			{
				ImGui::OpenPopup("MSBuild");
				this->msvcBuildWindow = false;
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
					ImGui::EndListBox();
				}

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

				if (ImGui::Button("Build Executable")) 
				{
					std::string msvcCommand = std::format("cmake --build \"{0}\" --config Release", ASSET_DIR + "bin/Release");

					std::cout << msvcCommand << std::endl;

					std::system(msvcCommand.c_str());

					msvcCommand = std::format("cmake -E copy_directory \"{0}\" \"{1}\"", std::filesystem::current_path().parent_path().string() + "/Release", this->projectPath);

					std::system(msvcCommand.c_str());
				}

				if (ImGui::Button("Exit Build Menu"))
				{
					this->buildWindow = false;
				}
			}

			if (ImGui::BeginPopupModal("Confirmation Window"))
			{
				ImGui::InputText("Scene Name: ", &this->sceneName);
				ImGui::Text("Are you sure you want to overwrite this scene?");
				if (ImGui::Button("Yes"))
				{
					if (K::Editor::GetCurrentScene()->GetSceneName() != this->sceneName)
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

			if (ImGui::BeginPopupModal("MSBuild"))
			{
				ImGui::Text("Are you sure you want to build with MSBuild?");

				if (ImGui::Button("Yes"))
				{
					K::SceneManager::currentScene->CreateEmptyScene();

					this->selectedGameObject = nullptr;

					this->UnloadComponents();

					std::string msvcCommand = std::format("cmake --build \"{0}\" --target Components", ASSET_DIR + "bin/Debug");

					std::system(msvcCommand.c_str());

					this->LoadComponents();

					K::SceneManager::LoadScene(0);

					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::Button("No"))
				{
					K::SceneManager::currentScene->CreateEmptyScene();

					this->selectedGameObject = nullptr;

					this->LoadComponents();

					K::SceneManager::LoadScene(0);

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / (ImGui::GetIO().Framerate), (ImGui::GetIO().Framerate));

			ImGui::Text("Active Materials: %i", K::materialManager.materials.size());

			for (auto temp : K::materialManager.materials) 
			{
				ImGui::Text("Material: %s Dependencies: %i", temp.first.c_str(), temp.second.dependencies);
			}

			ImGui::Text("Active Textures: %i", K::textureManager.textures.size());

			for (auto temp : K::textureManager.textures)
			{
				ImGui::Text("Texture: %s Dependencies: %i", temp.first.c_str(), temp.second.dependencies);
			}

			ImGui::Text("Number of GameObjects: %i", K::Editor::GetCurrentScene()->GetNumberOfObjects());

			ImGuiExtra();

			ImGui::End();
		}
	}

	void Editor::AddPreloadedTexture(std::string location) 
	{
		this->preloadedTextures.insert({ location, new K::Texture(location.c_str()) });
	}

	void Editor::LoadPreloadedTextures() 
	{
		for (auto temp : this->preloadedTextures) 
		{
			temp.second->Bind(0);
			temp.second->Unbind();
		}
	}

	void Editor::ImGuiContentBrowser() 
	{
		if (ImGui::Begin("K-Engine Content Browser")) 
		{
			if (ImGui::Button("Reload")) 
			{
				this->updateFiles = true;
			}

			if (this->currentDirectory.string() != ASSET_DIR && this->currentDirectory.string() + "/" != ASSET_DIR)
			{
				if (ImGui::Button("..."))
				{
					this->currentDirectory = this->currentDirectory.parent_path();
					this->updateFiles = true;
				}
			}

			float contentWidth = ImGui::GetContentRegionAvail().x;
			if (contentWidth <= 256.0f)
			{
				contentWidth = 256.0f;
			}
			int numberOfColumns = contentWidth / 256.0f;
			ImGui::Columns(numberOfColumns, "Content Columns", false);

			std::string asset_Dir = ASSET_DIR;

			if (this->updateFiles) 
			{
				this->files.clear();
				for (auto& p : std::filesystem::directory_iterator(this->currentDirectory))
				{
					this->files.push_back(p);
				}
				this->updateFiles = false;
			}

			for (auto file : this->files) 
			{
				std::string relativeLocation = file.path().string();
				relativeLocation.erase(relativeLocation.begin(), relativeLocation.begin() + asset_Dir.size());
				if (file.is_directory())
				{
					K::Texture* temp = this->preloadedTextures.at("textures/editor/file.png");
					if (ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)temp->GetViewID(), ImVec2(temp->GetWidth(), temp->GetHeight()), ImVec2(0, 1), ImVec2(1, 0)))
					{
						this->currentDirectory = file.path();
						this->updateFiles = true;
					}
				}
				else
				{
					if (file.is_regular_file())
					{
						numberOfColumns = contentWidth / 128.0f;
						if (file.path().extension() == ".png")
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
						else if (file.path().extension() == ".gif")
						{
							if (this->preloadedTextures.contains(relativeLocation))
							{
								K::Texture* temp = this->preloadedTextures.at(relativeLocation);
								ImGui::ImageButton(relativeLocation.c_str(), (void*)(intptr_t)temp->GetViewID(), ImVec2(128.0f, 128.0f), ImVec2(0, 1), ImVec2(1, 0));
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
						else if (file.path().extension() == ".JAWS")
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
							ImGui::ImageButton(file.path().string().c_str(), (void*)(intptr_t)unknown->GetID(), ImVec2(unknown->GetWidth(), unknown->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
						}
					}
				}
				ImGui::TextWrapped(relativeLocation.c_str());
				ImGui::NextColumn();
			}

			this->LoadPreloadedTextures();

			ImGui::End();
		}
	}

	bool Editor::Render() 
	{
		ImGuiBegin();

		ImGuiInspector();

		ImGuiHierarchy();

		if(!ASSET_DIR.empty())
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

	void Editor::SetDirectory(std::filesystem::path path) 
	{
		K::Editor::currentDirectory = path;
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
		temp += "###Hierarchy";
		if (ImGui::Begin(temp.c_str())) 
		{
			ImGui::Checkbox("is Paused", &this->GetCurrentScene()->isPaused);

			this->ParentChildrenHierarchy();

			ImGui::End();
		}
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
				K::GameObject* temp = new K::GameObject(*this->selectedGameObject);
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
				for(auto const& x : this->lst())
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
