#pragma once
#include "build.h"
#include "Component.h"
#include "Animator.h"
#include "PhysicsEngine.h"

namespace K
{
	class K_API Enemy : public K::Component
	{
	private:
		float time = 0.0f;
		float movementSpeed = 1.0f;
		K::Vector3 direction = K::Vector3(1.0f, 0.0f, 0.0f);
		K::Collider* col;
		K::Animator* animator;
		K::Sprite* sprite;
		std::string properties;
	public:
		Enemy();

		~Enemy();

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