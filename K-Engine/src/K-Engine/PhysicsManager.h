#pragma once
#include "build.h"
#include "Collider.h"

namespace K 
{
	class K_API PhysicsManager
	{
	private:
		inline static std::vector<K::Collider*> colliders;
	public:
		static void Attach(K::Collider* col);
		static void Remove(K::Collider* col);
		static bool IsColliding(K::Vector2 pos);
	};
}