#include "PhysicsEngine.h"

namespace K 
{
	std::vector<K::Collider*> K::Physics::colliders;

	void Physics::Remove(K::Collider* col)
	{
		int i = 0;
		for (K::Collider* other : K::Physics::colliders) 
		{
			if (other == col) 
			{
				break;
			}
			i++;
		}
		K::Physics::colliders.erase(K::Physics::colliders.begin() + i);
		K::Physics::colliders.shrink_to_fit();
	}

	void Physics::RemoveAll()
	{
		K::Physics::colliders.clear();
		K::Physics::colliders.shrink_to_fit();
	}

	void Physics::Attach(K::Collider* col)
	{
		Physics::colliders.push_back(col);
	}

	bool Physics::CanGetClosestPoint() 
	{
		int points = 0;
		for (int i = 0; i < Physics::colliders.size(); i++) 
		{
			if (Physics::colliders[i]->GetNumberOfPoints() > 0) 
			{
				return true;
			}
		}
		return false;
	}

	bool Physics::IsColliding(K::GameObject* parent) 
	{
		int index = -1;
		for (int i = 0; i < Physics::colliders.size(); i++) 
		{
			if (Physics::colliders[i]->parent == parent) 
			{
				index = i;
				break;
			}
		}
		if (index >= 0) 
		{
			return Physics::colliders[index]->IsColliding();
		}
		else 
		{
			return false;
		}
	}

	bool Physics::RemoveDuplicatesFromVectorArray(K::Vector3 a, K::Vector3 b) 
	{
		if (a.x == b.x && a.y == b.y && a.z == b.z) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool Physics::IsStatic(K::GameObject* parent) 
	{
		int index = -1;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (Physics::colliders[i]->parent == parent)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			return Physics::colliders[index]->IsStatic();
		}
		else
		{
			return false;
		}
	}

	std::vector<K::Vector3> Physics::GetClosestPoints(K::Vector3 position)
	{
		std::vector<K::Vector3> points;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
			{
				for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++) 
				{
					K::Vector3 J = *Physics::colliders[i]->PointOnLine(Physics::colliders[i]->GetLine(j)->point[0], Physics::colliders[i]->GetLine(j)->point[1], position);
					points.push_back(K::Vector3(J.x, J.z, J.y));
				}
			}
		}
		//auto it = std::unique(points.begin(), points.end(), RemoveDuplicatesFromVectorArray);
		//points.resize(std::distance(points.begin(), it));
		return points;
	}

	K::Vector3* Physics::GetCollisionResolution(K::Collider* col) 
	{
		K::Vector3* offsetAmount = new K::Vector3();
		if (col->colliderType == K::Collider::ColliderType::Circle) 
		{
			K::Vector3 position = *col->GetPosition();
			for (K::Vector3 J : K::Physics::GetClosestPoints(position))
			{
				K::Vector3 originToJ = J - position;
				K::Vector3 jToOrigin = position - J;
				if (originToJ.magnitude() < col->GetRadius())
				{
					jToOrigin.normalise();
					K::Vector3 up = K::Vector3(0.0f, 0.0f, 1.0f);
					K::Vector3 contactResolution = originToJ + (jToOrigin * col->GetRadius());
					*offsetAmount += contactResolution;
					col->SetIsColliding(true);
				}
			}
		}
		if (offsetAmount->magnitude() == 0.0f) 
		{
			col->SetIsColliding(false);
		}
		else
		{
			//std::cout << offsetAmount->x << " " << offsetAmount->y << " " << offsetAmount->z << std::endl;
		}
		return offsetAmount;
	}

	K::Vector3* Physics::GetClosestPoint(K::Vector3 position) 
	{
		int closestIndex = 0;
		float distance = INFINITY;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
			{
				K::Vector3 PJ = *Physics::colliders[i]->ClosestPointLineCollider(position) - position;
				if (PJ.magnitude() < distance)
				{
					distance = PJ.magnitude();
					closestIndex = i;
				}
			}
		}
		return Physics::colliders[closestIndex]->ClosestPointLineCollider(position);
	}
}