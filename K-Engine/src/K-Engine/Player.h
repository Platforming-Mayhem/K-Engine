#pragma once
#include "build.h"
#include "Component.h"

namespace K 
{
	class K_API Player : public K::Component
	{
	private:
		float movementSpeed = 1.0f;
		std::string properties;
	public:
		Player();

		~Player();

		float GetMovementSpeed();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}
