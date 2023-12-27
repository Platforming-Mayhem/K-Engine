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
		ContactPoint(K::Vector3 newPos, K::Vector3 newNorm)
		{
			this->position = newPos;
			this->normal = newNorm;
		}
	};

	class K_API Physics
	{
	private:
		static std::vector<K::Collider*> colliders;
	public:
		static void Remove(K::Collider* col);

		static void RemoveAll();

		static void Attach(K::Collider* col);

		static bool IsColliding(K::GameObject* parent);

		static bool CanGetClosestPoint();

		static bool IsInLayer(K::Collider* col, std::vector<K::Layer> avoidLayer);

		static bool Raycast(K::Vector3 origin, K::Vector3 direction, std::vector<K::Layer> avoidLayer, K::Collider** hit = nullptr);

		static bool IsStatic(K::GameObject* parent);

		static std::vector<K::ContactPoint> GetClosestPoints(K::Vector3 position);

		static K::Vector3* GetCollisionResolution(K::Collider* col);

		static K::Vector3* GetClosestPoint(K::Vector3 position);
	};
}