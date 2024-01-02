#pragma once
#include "build.h"
#include "Component.h"
#include "Texture.h"
#include "Time.h"
#include "InputManager.h"
#include "GameObject.h"

namespace K 
{
	class K_API Sprite : public K::Component
	{
	private:
		K::Texture* texture;
		int internalClock = 0;
		float chromaKeyColour[3];
		int frame = 0;
		bool canChromaKey = false;
		bool isPlaying = true;
		K::Vector3 lightDirection = K::Vector3(0.0f, -0.5f, 0.5f);
		std::string properties;
	public:
		ImGui::FileBrowser file;

		Sprite();

		~Sprite();

		K::Texture* GetTexture();

		void SetTexture(K::Texture* newTexture);

		void NextFrame();

		bool IsPlaying();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}