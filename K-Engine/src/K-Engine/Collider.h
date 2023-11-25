#pragma once
#include "build.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace K
{
	class K_API Collider : public K::Component
	{
	private:
		bool isStatic = true;
		std::string properties;
	public:
		Collider();

		~Collider();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value) override;

		void SetStaticState(bool state);

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}