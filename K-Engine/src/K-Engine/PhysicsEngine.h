#pragma once
#include "build.h"
#include "Collider.h"
#include "Transform.h"

namespace K 
{
	class K_API Physics
	{
	private:
		static std::vector<K::Collider*> colliders;
	public:
		static void Remove(K::Collider* col);

		static void RemoveAll();

		static void Attach(K::Collider* col);

		static bool RemoveDuplicatesFromVectorArray(K::Vector3 a, K::Vector3 b);

		static bool IsColliding(K::GameObject* parent);

		static bool CanGetClosestPoint();

		static std::vector<K::Vector3> GetClosestPoints(K::Vector3 position);

		static K::Vector3* GetCollisionResolution(K::Collider* col);

		static K::Vector3* GetClosestPoint(K::Vector3 position);
	};
}