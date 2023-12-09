#include "PhysicsEngine.h"

namespace K 
{
	std::vector<K::Collider*> K::Physics::colliders;

	void Physics::Remove(K::Collider* col)
	{
		std::cout << "Removing Collider..." << std::endl;
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
		std::cout << "Removing All Colliders..." << std::endl;
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

	std::vector<K::ContactPoint> Physics::GetClosestPoints(K::Vector3 position)
	{
		std::vector<K::ContactPoint> points;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
			{
				for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++) 
				{
					K::Vector3 J = *Physics::colliders[i]->PointOnLine(Physics::colliders[i]->GetLine(j)->point[0], Physics::colliders[i]->GetLine(j)->point[1], position);
					K::Vector3 J2 = K::Vector3(J.x, 0.0f, J.y);
					K::Vector3 Normal = *Physics::colliders[i]->GetNormal(Physics::colliders[i]->GetLine(j)->point[0], Physics::colliders[i]->GetLine(j)->point[1]);
					points.push_back(K::ContactPoint(J2, Normal));
				}
			}
		}
		return points;
	}

	K::Vector3* Physics::GetCollisionResolution(K::Collider* col) 
	{
		K::Vector3* offsetAmount = new K::Vector3();
		if (col->colliderType == K::Collider::ColliderType::Circle) 
		{
			K::Vector3 position = *col->GetPosition();
			for (K::ContactPoint J : K::Physics::GetClosestPoints(position))
			{
				K::Vector3 originToJ = J.normal * -(J.position - position).magnitude();
				K::Vector3 normal = J.normal;
				if (originToJ.magnitude() < col->GetRadius())
				{
					K::Vector3 contactResolution = originToJ + (normal * col->GetRadius());
					*offsetAmount += contactResolution;
				}
			}
		}
		if (offsetAmount->magnitude() <= 0.0f)
		{
			col->SetIsColliding(false);
		}
		else 
		{
			K::Vector3 up = K::Vector3(0.0f, 0.0f, 1.0f);
			K::Vector3 offsetNormalized = *offsetAmount;
			offsetNormalized.normalise();
			if (K::Vector3::DotProduct(offsetNormalized, up) != 0.0f && K::Vector3::DotProduct(offsetNormalized, up) > 0.0f)
			{
				col->SetIsColliding(true);
			}
			else 
			{
				col->SetIsColliding(false);
			}
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