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
		K::Texture* normalTexture;
		float internalClock = 0;
		float chromaKeyColour[3];
		int frame = 0;
		bool hasNormal = false;
		bool canChromaKey = false;
		bool isPlaying = true;
		std::string properties;

	public:

		Sprite();

		~Sprite();

		K::Texture* GetTexture();

		void SetColorTexture(const char* value);

		void SetNormalTexture(const char* value);

		void SetColorTexture(unsigned int resource);

		void SetNormalTexture(unsigned int resource);

		void SetTexture(K::Texture* newTexture, bool reScale = true);

		void ResetFrame();

		void NextFrame();

		int GetFrameNumber();

		bool IsPlaying();

		void RenderInit() override;

		void Init() override;

		void RenderBind() override;

		void RenderUnbind() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}