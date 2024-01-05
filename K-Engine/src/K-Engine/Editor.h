#pragma once
#include "build.h"
#include "Scene.h"
#include "Material.h"
#include "Window.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "Collider.h"
#include "Animator.h"
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
		K::Window* window;
		ImGui::FileBrowser file;
		static K::Material* material;
		static K::Scene* currentScene;
		static K::GameObject* selectedGameObject;
	public:

		std::map<std::string, IFactory*> lst { {typeid(K::Sprite).name() , new K::Factory<K::Sprite>} , {typeid(K::Player).name() , new K::Factory<K::Player>} ,{typeid(K::Mesh).name() , new K::Factory<K::Mesh>} ,{typeid(K::Camera).name() , new K::Factory<K::Camera>} ,{typeid(K::Collider).name() , new K::Factory<K::Collider>} ,{typeid(K::Animator).name() , new K::Factory<K::Animator>}, {typeid(K::Enemy).name() , new K::Factory<K::Enemy>} };

		Editor(K::Window* window, K::Scene* scene, K::Material* material);

		virtual ~Editor();

		void ImGuiHierarchy();

		void ImGuiExtra();

		bool Render();

		static K::Material* GetMaterial();

		static void Delete(K::GameObject* temp);

		static K::GameObject* GetSelectedGameObject();

		static K::Scene* GetScene();
	};
}