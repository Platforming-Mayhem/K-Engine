#pragma once
#include "build.h"
#include "SceneManager.h"
#include "Material.h"
#include "Window.h"
#include "Texture.h"

namespace K 
{
	struct K_API IFactory
	{ 
		virtual K::Component* create() = 0; 

		virtual const char* getName() = 0;
	};

	template< typename Type > struct K_API Factory : public IFactory
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
		std::filesystem::path currentDirectory;
		K::RenderTexture* viewport;
		std::map<std::string, K::Texture*> preloadedTextures;
		K::Window* window;
		bool buildWindow;
		bool confirmationWindow = false;
		int selectedScene = -1;
		static K::Material* material;
		static K::SceneManager* sceneManager;
	public:

		static K::GameObject* selectedGameObject;

		static std::map<std::string, IFactory*> lst;

		static std::vector<K::GameObject*> deleteArray;

		Editor(K::Window* window, K::SceneManager* sceneManager, K::Material* material);

		virtual ~Editor();

		void AddPreloadedTexture(std::string location, GLenum type);

		void LoadPreloadedTextures();

		void ParentChildrenHierarchy();

		void ShowChildren(K::GameObject* current);

		void ImGuiInspector();

		void ImGuiHierarchy();

		void ImGuiViewport();

		void ImGuiContentBrowser();

		void ImGuiExtra();

		void ImGuiBegin();

		void ImGuiEnd();

		bool Render();

		K::RenderTexture* GetViewport();

		static K::Material* GetMaterial();

		static void Delete(K::GameObject* target);

		static K::GameObject* GetSelectedGameObject();

		static K::Scene* GetCurrentScene();
	};
}