#pragma once
#include "build.h"
#include "SceneManager.h"
#include "Material.h"
#include "Window.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Collider.h"
#include "Animator.h"
#include "Component.h"
#include "Fox.h"

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
		K::Window* window;
		ImGui::FileBrowser file;
		bool buildWindow;
		bool saveWindow;
		int selectedScene = -1;
		static K::Material* material;
		static K::SceneManager* sceneManager;
		static K::GameObject* selectedGameObject;
	public:

		static std::map<std::string, IFactory*> lst;

		Editor(K::Window* window, K::SceneManager* sceneManager, K::Material* material);

		virtual ~Editor();

		void ImGuiHierarchy();

		void ImGuiExtra();

		bool Render();

		static K::Material* GetMaterial();

		static void Delete(K::GameObject* temp);

		static K::GameObject* GetSelectedGameObject();

		static K::Scene* GetCurrentScene();
	};
}