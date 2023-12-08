#pragma once
#include "build.h"
#include "Component.h"
#include "Sprite.h"

namespace K 
{
	class K_API Animator : public K::Component 
	{
	private:
		std::string properties;
		std::vector<K::Texture*> animations;
		int selectedTexture = 0;
	public:
		ImGui::FileBrowser file;

		Animator();

		~Animator();

		void PlayAnimation(int index, K::Sprite* currentSprite);

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