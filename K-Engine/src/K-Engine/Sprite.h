#pragma once
#include "build.h"
#include "Component.h"
#include "Texture.h"
#include "Time.h"

namespace K 
{
	class K_API Sprite : public K::Component
	{
	private:
		K::Texture* texture;
		float internalClock = 0.0f;
		int frame = 0;
	public:
		ImGui::FileBrowser file;

		Sprite(K::Texture* newTexture = new K::Texture(WATERMARK, GL_TEXTURE_2D));

		~Sprite();

		void SetTexture(K::Texture* newTexture);

		void NextFrame();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value) override;

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}