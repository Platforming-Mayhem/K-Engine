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
		static void Attach(K::Collider* col);

		static bool CanGetClosestPoint();

		static K::Vector3* GetClosestPoint(K::Vector3 position);
	};
}