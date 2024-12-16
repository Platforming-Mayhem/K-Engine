#pragma once
#include <K_Engine.h>

namespace K 
{
	K_API extern K::Window* window;

	extern K::RenderTexture* renderTex;

	extern K::Material* editorMat;

	class K_API InputManager 
	{
	private:
		static std::unordered_map<int, int> keys;
	public:
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static int GetKey(int key);

		static bool IsKeyPressed(int key);

		static bool IsKeyPressedDown(int key);

		static bool IsKeyReleased(int key);
		
		static K::Vector3 GetMousePosition();

		static K::Vector3 ConvertToClipPosition(K::Vector3 position);

		static K::Vector3 GetWorldMouseDirection();

		static K::GameObject* PickGameObject();
	};
}