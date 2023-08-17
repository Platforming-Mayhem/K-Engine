#pragma once
#include "build.h"
#include "Scene.h"
#include "Sprite.h"
#include "Player.h"
#include "Collider.h"

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
		K::Shader* shader;
		std::vector<IFactory*> lst;
		K::Window* window;
		static K::GameObject* selectedGameObject;
	public:

		Editor(K::Window* window, K::Scene* scene, K::Shader* shader);

		virtual ~Editor();

		void ImGuiExtra();

		void Render();

		static K::GameObject* GetSelectedGameObject();
	};
}