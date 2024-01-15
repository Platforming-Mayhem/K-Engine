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
				K::Physics::colliders.erase(K::Physics::colliders.begin() + i);
				K::Physics::colliders.shrink_to_fit();
				break;
			}
			i++;
		}
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
		for (K::Collider* col : Physics::colliders)
		{
			if (col->parent == parent) 
			{
				return col->IsColliding();
			}
		}
		return false;
	}

	bool Physics::IsInLayer(K::Collider* col, std::vector<K::Layer> avoidLayer)
	{
		bool isInLayer = true;
		for (K::Layer layer : avoidLayer)
		{
			if (col->parent->layer == layer.layer)
			{
				isInLayer = false;
				return isInLayer;
			}
		}
		return isInLayer;
	}

	bool Physics::Hitbox(K::Vector3 bottomLeft, K::Vector3 topRight, std::vector<K::Layer> avoidLayer, K::Collider** hit)
	{
		K::Vector3 centre = (topRight + bottomLeft) * 0.5f;
		for (K::ContactPoint contact : Physics::GetClosestPoints(centre, avoidLayer))
		{
			if ((contact.position - topRight).z < 0.0f && (contact.position - topRight).x < 0.0f)
			{
				if ((contact.position - bottomLeft).z > 0.0f && (contact.position - bottomLeft).x > 0.0f)
				{
					if (hit != nullptr)
						*hit = contact.other;
					return true;
				}
			}
		}
		return false;
	}

	//Direction up is K::Vector3(0.0f, 1.0f, 0.0f);
	bool Physics::Raycast(K::Vector3 origin, K::Vector3 direction, std::vector<K::Layer> avoidLayer, K::Collider** hit)
	{
		K::Vector3 A = K::Vector3(origin.x, origin.z, 0.0f);
		K::Vector3 B = A + direction;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (K::Physics::IsInLayer(Physics::colliders[i], avoidLayer))
			{
				if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line)
				{
					for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++)
					{
						K::Vector3 C = Physics::colliders[i]->GetLine(j)->point[0];
						K::Vector3 D = Physics::colliders[i]->GetLine(j)->point[1];
						float x = (-(A.x * B.y - B.x * A.y) * (D.x - C.x) + (C.x * D.y - D.x * C.y) * (B.x - A.x)) / (-(C.y - D.y) * (B.x - A.x) + (A.y - B.y) * (D.x - C.x));
						float y = (-(A.y - B.y) * x - (A.x * B.y - B.x * A.y)) / (B.x - A.x);
						K::Vector3 J = K::Vector3(x, y, 0.0f);
						K::Vector3 CJ = (J - C).normalise();
						K::Vector3 DJ = (J - D).normalise();
						K::Vector3 CD = (C - D).normalise();
						float U = K::Vector3::DotProduct(DJ, CD);
						float V = K::Vector3::DotProduct(CJ, CD);
						if (U > 0.0f && V < 0.0f)
						{
							if(hit != nullptr)
								*hit = Physics::colliders[i];
							return true;
						}
					}
				}
				else if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Circle)
				{
					K::Vector3 J = *Physics::colliders[i]->PointOnLine(A, B, *Physics::colliders[i]->GetPosition());
					K::Vector3 J2 = K::Vector3(J.x, 0.0f, J.y);
					K::Vector3 offset = J2 - *Physics::colliders[i]->GetPosition();
					if (offset.magnitude() < Physics::colliders[i]->GetRadius())
					{
						if (hit != nullptr)
							*hit = Physics::colliders[i];
						return true;
					}
				}
			}
		}
		hit = nullptr;
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

	std::vector<K::ContactPoint> Physics::GetClosestPoints(K::Vector3 position, std::vector<K::Layer> avoidLayer)
	{
		std::vector<K::ContactPoint> points;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (K::Physics::IsInLayer(Physics::colliders[i], avoidLayer)) 
			{
				if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line && Physics::colliders[i]->GetNumberOfPoints() > 0)
				{
					for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++)
					{
						K::Vector3 J = *Physics::colliders[i]->PointOnLine(Physics::colliders[i]->GetLine(j)->point[0], Physics::colliders[i]->GetLine(j)->point[1], position);
						K::Vector3 J2 = K::Vector3(J.x, 0.0f, J.y);
						K::Vector3 Normal = *Physics::colliders[i]->GetNormal(Physics::colliders[i]->GetLine(j)->point[0], Physics::colliders[i]->GetLine(j)->point[1]);
						points.push_back(K::ContactPoint(J2, Normal, Physics::colliders[i]));
					}
				}
				else if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Circle)
				{
					K::Vector3 otherPosition = *Physics::colliders[i]->GetPosition();
					K::Vector3 J = K::Vector3(position.x - otherPosition.x, 0.0f, position.z - otherPosition.z).normalise();
					K::Vector3 Jtemp = otherPosition + (J * Physics::colliders[i]->GetRadius());
					K::Vector3 J2 = K::Vector3(Jtemp.x, 0.0f, Jtemp.z);
					K::Vector3 Normal = J;
					points.push_back(K::ContactPoint(J2, Normal, Physics::colliders[i]));
				}
			}
		}
		return points;
	}

	K::Vector3* Physics::GetCollisionResolution(K::Collider* col) 
	{
		K::Vector3* offsetAmount = new K::Vector3();
		int count = 0;
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
						count++;
					}
				}
			}
		}
		if (offsetAmount->magnitude() <= 0.0f || count == 0) 
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