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

	bool Physics::Raycast(K::Vector3 origin, K::Vector3 direction) 
	{
		K::Vector3 A = origin;
		K::Vector3 B = origin + direction;
		for (int i = 0; i < Physics::colliders.size(); i++) 
		{
			if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
			{
				for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++)
				{
					K::Vector3 E = Physics::colliders[i]->GetLine(j)->point[0];
					K::Vector3 F = Physics::colliders[i]->GetLine(j)->point[1];
					float y = ((A.z - B.z) * (E.x * F.y - F.x * E.y) / (B.x - A.x) * (E.y - F.y) - (((A.x * B.z) - (B.x * A.z)) / (B.x - A.x))) / (1 + ((A.z - B.z) * (-F.x + E.x))/((B.x - A.x) * (E.y - F.y)));
					float x = (-(F.x - E.x) * y - (E.x * F.y - F.x * E.y)) / (E.y - F.y);
					K::Vector3 J = K::Vector3(x, y, 0.0f);
					K::Vector3 AJ = (J - A).normalise();
					K::Vector3 BJ = (J - B).normalise();
					K::Vector3 AB = (A - B).normalise();
					float U = K::Vector3::DotProduct(BJ, AB);
					float V = K::Vector3::DotProduct(AJ, AB);
					if (U > 0.0f && V < 0.0f)
					{
						return true;
					}
				}
			}
			else if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Circle)
			{
				
			}
		}
		return false;
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
			else if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Circle) 
			{
				K::Vector3 otherPosition = *Physics::colliders[i]->GetPosition();
				K::Vector3 J = K::Vector3(position.x - otherPosition.x, 0.0f, position.z - otherPosition.z).normalise();
				K::Vector3 Jtemp = otherPosition + (J * Physics::colliders[i]->GetRadius());
				K::Vector3 J2 = K::Vector3(Jtemp.x, 0.0f, Jtemp.z);
				K::Vector3 Normal = J;
				points.push_back(K::ContactPoint(J2, Normal));
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
				K::Vector3 originToJ = J.normal.normalise() * -(J.position - position).magnitude();
				K::Vector3 normal = J.normal.normalise();
				if (originToJ.magnitude() < col->GetRadius())
				{
					K::Vector3 up = K::Vector3(0.0f, 0.0f, 1.0f);
					K::Vector3 right = K::Vector3(1.0f, 0.0f, 0.0f);
					float angle = K::Vector3::DotProduct(normal, right);
					float angle1 = K::Vector3::DotProduct(normal, up);

					K::Vector3 contactResolution = originToJ + (normal * col->GetRadius());
					*offsetAmount += contactResolution;

					if (angle < 0.8f && angle > -0.8f && angle1 > 0.0f)
					{
						col->SetIsColliding(true);
					}
					else
					{
						col->SetIsColliding(false);
					}
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