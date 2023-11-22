#pragma once
#include "build.h"
#include "Scene.h"
#include "Material.h"
#include "Window.h"
#include "Sprite.h"
#include "Player.h"
#include "Camera.h"
#include "Collider.h"
#include "Component.h"

namespace K 
{
	struct IFactory { virtual K::Component* create() = 0; };

	template< typename Type > struct Factory : public IFactory {
		virtual Type* create() {
			return new Type();
		}
	};

	class K_API Editor
	{
	private:
		K::Scene* currentScene;
		K::Material* material;
		K::Window* window;
		ImGui::FileBrowser file;
		static K::GameObject* selectedGameObject;
	public:

		std::vector<IFactory*> lst;

		Editor(K::Window* window, K::Scene* scene, K::Material* material);

		virtual ~Editor();

		void ImGuiHierarchy();

		void ImGuiExtra();

		bool Render();

		K::Material* GetMaterial();

		static K::GameObject* GetSelectedGameObject();
	};
}