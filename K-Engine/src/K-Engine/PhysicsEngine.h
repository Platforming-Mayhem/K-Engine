#pragma once
#include "build.h"
#include "Collider.h"
#include "Transform.h"

namespace K 
{
	struct K_API ContactPoint
	{
		K::Vector3 position = K::Vector3();
		K::Vector3 normal = K::Vector3();
		K::Collider* other = nullptr;
		ContactPoint(K::Vector3 newPos, K::Vector3 newNorm, K::Collider* other = nullptr)
		{
			this->position = newPos;
			this->normal = newNorm;
			this->other = other;
		}

		~ContactPoint() 
		{
			this->other = nullptr;
			//std::cout << "Destroy ContactPoint" << std::endl;
		}
	};

	class K_API Physics
	{
	private:
		static std::vector<K::Collider*> colliders;

		static const int fixedTimeStep = 120;
	public:
		static float GetFixedTimeStep();

		static void Remove(K::Collider* col);

		static void RemoveAll();

		static void Attach(K::Collider* col);

		static bool IsColliding(K::GameObject* parent);

		static bool CanGetClosestPoint();

		static bool IsInLayer(K::Collider* col, std::vector<K::Layer> avoidLayer);

		static bool Hitbox(K::Vector3 bottomLeft, K::Vector3 topRight, std::vector<K::Layer> avoidLayer, K::Collider** hit = nullptr);

		static bool Raycast(K::Vector3 origin, K::Vector3 direction, std::vector<K::Layer> avoidLayer, K::Collider** hit = nullptr);

		static bool IsStatic(K::GameObject* parent);

		static std::vector<K::ContactPoint> GetClosestPoints(K::Vector3 position, std::vector<K::Layer> avoidLayer = {});

		static K::Vector3 GetCollisionResolution(K::Collider* col, std::vector<K::Layer> avoidLayer = {});

		static K::Vector3 GetClosestPoint(K::Vector3 position, std::vector<K::Layer> avoidLayer = {}, K::Collider** hit = nullptr);
	};
}