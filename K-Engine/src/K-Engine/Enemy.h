#pragma once
#include "build.h"
#include "Time.h"
#include "Component.h"
#include "Animator.h"
#include "PhysicsEngine.h"

namespace K
{
	class K_API Enemy : public K::Component
	{
	private:
		float time = 0.0f;
		float jumpTime = 0.0f;
		float decidingTime = 1.0f;
		int r = 0;
		K::Vector3 direction;
		K::Collider* col;
		K::Animator* animator;
		K::Sprite* sprite;
		std::string properties;
	public:
		bool isAttacking = false;
		bool isJumping = false;
		float movementSpeed = 1.0f;
		int minAttackFrame = 0;
		int maxAttackFrame = 0;

		Enemy();

		~Enemy();

		void Attack();

		void AvoidFalling();

		void AvoidWalls();

		void Move();

		void Gravity();

		void Jump();

		void JumpUpdate();

		void ChooseDirection();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void RaycastVisualiser();

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}