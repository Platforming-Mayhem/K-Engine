#pragma once
#include "build.h"
#include "SceneManager.h"
#include "Material.h"
#include "Window.h"
#include "Sprite.h"
#include "Player.h"
#include "Ship.h"
#include "Camera.h"
#include "Collider.h"
#include "Animator.h"
#include "FollowPlayer.h"
#include "Timer.h"
#include "Move.h"
#include "Light.h"
#include "TriggerDeath.h"
#include "Component.h"

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
		K::RenderTexture* viewport;
		K::Window* window;
		bool buildWindow;
		bool saveWindow;
		int selectedScene = -1;
		static K::Material* material;
		static K::SceneManager* sceneManager;
	public:

		static K::GameObject* selectedGameObject;

		static std::map<std::string, IFactory*> lst;

		static std::vector<K::GameObject*> deleteArray;

		Editor(K::Window* window, K::SceneManager* sceneManager, K::Material* material);

		virtual ~Editor();

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