#pragma once
#include "build.h"
#include "Component.h"

namespace K 
{
	class K_API Player : public K::Component
	{
	private:
		float movementSpeed = 1.0f;
	public:
		Player();

		~Player();

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
