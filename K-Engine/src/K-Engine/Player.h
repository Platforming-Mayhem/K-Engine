#pragma once
#include "build.h"
#include "Component.h"
#include "Animator.h"
#include "PhysicsEngine.h"

namespace K 
{
	class K_API Player : public K::Component
	{
	private:
		bool hasGravity = false;
		bool isJumping = false;
		bool isAttacking = false;
		float movementSpeed = 1.0f;
		float time = 0.0f;
		float jumpTime = 0.0f;
		float accelerationTime = 0.0f;
		float decelerationTime = 0.0f;
		float attackDirection = 1.0f;
		K::Vector3* direction;
		K::Animator* animator;
		K::Collider* col;
		K::Collider* hit = nullptr;
		K::Sprite* sprite;
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
