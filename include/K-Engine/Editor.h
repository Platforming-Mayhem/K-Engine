#pragma once
#include "build.h"
#include "SceneManager.h"
#include "Material.h"
#include "Window.h"
#include "RenderTexture.h"
#include "Texture.h"

namespace K 
{
	struct K_API IFactory
	{ 
		virtual K::Component* create() = 0; 

		virtual const char* getName() = 0;

		virtual ~IFactory()
		{
			//std::cout << "IFactory Destroyed" << std::endl;
		}
	};

	template< typename Type > struct Factory : public IFactory
	{
		virtual Type* create() 
		{
			return new Type();
		}

		virtual const char* getName() 
		{
			return typeid(Type).name();
		}
	};

	class K_API Editor
	{
	private:
		std::vector<std::string> editorTextures;
		std::string sceneName;
		std::string projectPath;

		K::RenderTexture* viewport;

		std::unordered_map<std::string, K::Texture*> preloadedTextures;

		K::Window* window;
		
		#if _WIN32

		HINSTANCE componentsLibrary = NULL;
		
		#elif __unix__
		
		void* componentsLibrary = nullptr;
		
		#endif

		bool buildWindow = false;
		bool confirmationWindow = false;
		bool projectLoadWindow = false;
		bool msvcBuildWindow = false;
		bool doesIniExist = false;

		int selectedScene = -1;

		static K::SceneManager* sceneManager;

		static std::filesystem::path currentDirectory;

	public:

		static float windowScaleFactor;

		static float offsetX;

		static float offsetY;

		static K::GameObject* selectedGameObject;

		static K::Matrix4x4 projectionMatrix;

		static K::Matrix4x4 viewMatrix;

		static K::Vector3* cameraPosition;

		static std::unordered_map<std::string, IFactory*>& lst();

		static std::vector<K::GameObject*> deleteArray;

		Editor(K::Window* window, K::SceneManager* sceneManager);

		virtual ~Editor();

		void AddPreloadedTexture(std::string location, bool isEditorTexture);

		void LoadPreloadedTextures();

		void LoadComponents();

		void UnloadComponents();

		void ParentChildrenHierarchy();

		void ShowChildren(K::GameObject* current);

		void ImGuiInspector();

		void ImGuiHierarchy();

		void ImGuiViewport();

		void ImGuiContentBrowser();

		void ImGuiExtra();

		void ImGuiBegin();

		void ImGuiEnd();

		static ImGuiContext* GetImGuiContext();

		bool Render();

		K::RenderTexture* GetViewport();

		static void SetDirectory(std::filesystem::path path);

		static void Delete(K::GameObject* target);

		static K::GameObject* GetSelectedGameObject();

		static K::Scene* GetCurrentScene();
	};

	struct ParentRegister
	{
		virtual ~ParentRegister(){}
	};

	class K_API ComponentManager
	{
		private:

		ComponentManager(){}

		~ComponentManager()
		{
			std::cout << "Destroy Component Manager" << std::endl;
		}

		public:

		static ComponentManager* componentManager;

		std::vector<ParentRegister*> registerLibrary;
		
		ComponentManager(ComponentManager &other) = delete;

		void operator=(const ComponentManager&) = delete;

		static ComponentManager* GetInstance() 
		{
			if (K::ComponentManager::componentManager == nullptr)
				K::ComponentManager::componentManager = new K::ComponentManager();
			return K::ComponentManager::componentManager;
		}

		static void RemoveInstance() 
		{
			delete K::ComponentManager::componentManager;
			K::ComponentManager::componentManager = nullptr;
		}
	};

	template<typename T> struct Register : ParentRegister
	{
		Register()
		{
			K::IFactory* tempFactory = new K::Factory<T>;
			std::string tempName = typeid(T).name();

			K::ComponentManager* comp = K::ComponentManager::GetInstance();

			comp->registerLibrary.push_back(this);

			#if __unix__
			int status;
			std::string demangledName = std::string("class ") + abi::__cxa_demangle(tempName.c_str(), NULL, NULL, &status);

			std::cout << demangledName << std::endl;

			K::Editor::lst().insert({ demangledName, tempFactory });
			#else
			K::Editor::lst().insert({ tempName, tempFactory });
			#endif
		}

		~Register()
		{
			std::string tempName = typeid(T).name();
			#if __unix__
			int status;
			std::string demangledName = std::string("class ") + abi::__cxa_demangle(tempName.c_str(), NULL, NULL, &status);
			delete K::Editor::lst().at(demangledName);
			K::Editor::lst().erase(demangledName);
			std::cout << "Destructed Register Class:" << demangledName << std::endl;
			#else
			delete K::Editor::lst().at(tempName);
			K::Editor::lst().erase(tempName);
			std::cout << "Destructed Register Class:" << tempName << std::endl;
			#endif
		}
	};

	#define REGISTER(Type) Register<Type>* s_##Type##Registered = new Register<Type>();
}
