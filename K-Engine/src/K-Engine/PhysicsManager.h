#pragma once
#include "build.h"
#include "Collider.h"
#include "GameObject.h"

namespace K 
{
	class K_API PhysicsManager
	{
	public:
		inline static std::vector<K::Collider*> colliders;
		static void Attach(K::Collider* col);
		static void Remove(K::Collider* col);
	};
}