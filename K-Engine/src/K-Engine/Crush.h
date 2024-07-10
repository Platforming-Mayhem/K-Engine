#pragma once
#include "build.h"
#include "Component.h"
#include "PhysicsEngine.h"

namespace K 
{
	class K_API Crush : public K::Component
	{
	private:
		K::Vector3 origin;

		K::Vector3* offset;

		float crushTime = 0.0f;

		bool crush = false;

		std::string properties;
	public:
		Crush();

		~Crush();

		float EaseInPow(float value, int pow);

		void VisualDebug();

		void Init() override;

		void UpdateEditor() override;

		void Update() override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;
	};
}