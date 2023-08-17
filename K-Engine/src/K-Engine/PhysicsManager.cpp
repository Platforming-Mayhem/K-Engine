#include "PhysicsManager.h"

namespace K 
{
	void PhysicsManager::Attach(K::Collider* col) 
	{
		K::PhysicsManager::colliders.push_back(col);
	}
	void PhysicsManager::Remove(K::Collider* col) 
	{
		delete col;
	}
	bool PhysicsManager::IsColliding(K::Vector2 pos) 
	{
		for (K::Collider* col : K::PhysicsManager::colliders) 
		{
			if (col->IsCollidingAtPoint(pos)) 
			{
				return true;
			}
		}
		return false;
	}
}