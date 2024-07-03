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
		K::Texture* currentTexture;
		K::Texture* initialTexture;
		int selectedTexture = 0;
	public:

		Animator();

		~Animator();

		void PlayAnimation(int index, K::Sprite* currentSprite, bool reScale = true);

		void AssignTexture(const char* value);

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;
	};
}