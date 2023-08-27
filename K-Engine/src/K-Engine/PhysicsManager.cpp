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
	bool PhysicsManager::IsColliding(K::Collider* other)
	{
		for (K::Collider* col : K::PhysicsManager::colliders) 
		{
			for (int i = 0; i < other->parent->GetMesh()->vertices.size(); i++) 
			{
				if (other->GetID() != col->GetID())
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(other->parent->GetMesh()->vertices[i].position, temp, other->parent->GetTransform()->modelMatrix);
					if (col->IsCollidingWithTriangle(temp))
					{
						return true;
					}
				}
			}
			for (int i = 0; i < col->parent->GetMesh()->vertices.size(); i++) 
			{
				if (col->GetID() != other->GetID()) 
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(col->parent->GetMesh()->vertices[i].position, temp, col->parent->GetTransform()->modelMatrix);
					if (other->IsCollidingWithTriangle(temp))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
}