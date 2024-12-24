#pragma once
#include "K-Engine/Component.h"
#include "K-Engine/Time.h"
#include "K-Engine/SceneManager.h"

namespace K 
{
	class Timer : public K::Component 
	{
	private:
		std::string properties;
		bool startCountDown;
		float countDown = 1.0f;
	public:

		Timer();

		~Timer();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;
	};
}