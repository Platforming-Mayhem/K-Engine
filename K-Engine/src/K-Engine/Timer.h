#pragma once
#include "build.h"
#include "Component.h"
#include "Time.h"
#include "SceneManager.h"

namespace K 
{
	class K_API Timer : public K::Component 
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