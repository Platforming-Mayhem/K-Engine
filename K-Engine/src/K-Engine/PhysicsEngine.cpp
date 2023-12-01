#include "PhysicsEngine.h"

namespace K 
{
	std::vector<K::Collider*> K::Physics::colliders;

	void Physics::Attach(K::Collider* col)
	{
		Physics::colliders.push_back(col);
	}

	bool Physics::CanGetClosestPoint() 
	{
		int points = 0;
		for (int i = 0; i < Physics::colliders.size(); i++) 
		{
			points += Physics::colliders[i]->GetNumberOfPoints();
		}
		if (points == 0)
		{
			return false;
		}
		else 
		{
			return true;
		}
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

	std::vector<K::Vector3> Physics::GetClosestPoints(K::Vector3 position)
	{
		std::vector<K::Vector3> points;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
			{
				points.push_back(*Physics::colliders[i]->ClosestPointLineCollider(position));
			}
		}
		return points;
	}

	K::Vector3* Physics::GetCollisionResolution(K::Collider* col) 
	{
		K::Vector3* offsetAmount = new K::Vector3();
		if (col->colliderType == K::Collider::ColliderType::Circle) 
		{
			for (K::Vector3 p1 : K::Physics::GetClosestPoints(*col->parent->GetTransform()->position)) 
			{
				K::Vector3 J = p1;
				K::Vector3 originToJ = J - *col->parent->GetTransform()->position;
				K::Vector3 jToOrigin = *col->parent->GetTransform()->position - J;
				if (originToJ.magnitude() < col->GetRadius())
				{
					jToOrigin.normalise();
					K::Vector3 contactResolution = originToJ + (jToOrigin * col->GetRadius());
					*offsetAmount += new K::Vector3(contactResolution);
					col->SetIsColliding(true);
				}
			}
		}
		if (offsetAmount->magnitude() <= 0.0f) 
		{
			col->SetIsColliding(false);
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