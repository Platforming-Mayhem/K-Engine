#pragma once
#include "build.h"
#include "Component.h"
#include "Time.h"
#include "PhysicsEngine.h"
#include "Editor.h"

namespace K 
{
	class K_API Bullet 
	{
	private:
		K::Vector3 origin;
		K::Vector3 direction;
		float timeElapsed = 0.0f;
		float lifeTime = 1.0f;
		float radius = 1.0f;
	public:

		Bullet(K::Vector3 origin, K::Vector3 direction, float lifeTime = 1.0f, float radius = 1.0f) 
		{
			this->origin = origin;
			this->timeElapsed = 0.0f;
			this->lifeTime = lifeTime;
			this->direction = direction;
			this->radius = radius;
		}

		~Bullet() 
		{

		}

		void Update() 
		{
			if (this->timeElapsed >= this->lifeTime) 
			{
				delete this;
			}
			else 
			{
				K::Collider* temp = nullptr;
				K::Physics::Raycast(this->GetLocation(), this->direction, { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }, &temp);
				K::Vector3 displacement = *temp->GetPosition() - this->GetLocation();
				displacement *= K::Vector3(1.0f, 0.0f, 1.0f);
				if (displacement.magnitude() <= this->radius) 
				{
					K::Editor::Delete(temp->parent);
				}
				this->timeElapsed += K::Time::deltaTime();
			}
		}

		K::Vector3 GetLocation() 
		{
			return this->origin + (this->direction * this->timeElapsed);
		}
	};

	class K_API Shooter : public K::Component 
	{
	private:
		K::Collider* col;
		float projectileSpeed = 1.0f;
		std::string properties;
	public:

		Shooter();

		~Shooter();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}