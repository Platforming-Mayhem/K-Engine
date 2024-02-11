#include "PhysicsEngine.h"

namespace K 
{
	std::vector<K::Collider*> K::Physics::colliders;

	void Physics::Remove(K::Collider* col)
	{
		std::cout << "Removing Collider..." << std::endl;
		for (int i = 0; i < K::Physics::colliders.size(); i++)
		{
			if (K::Physics::colliders[i] == col)
			{
				K::Physics::colliders.erase(K::Physics::colliders.begin() + i);
				K::Physics::colliders.shrink_to_fit();
				break;
			}
		}
		std::cout << "Removed Collider" << std::endl;
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
		for (int i = 0; i < avoidLayer.size(); i++)
		{
			if (col->parent->layer == avoidLayer[i].layer)
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
		K::Vector3 C = K::Vector3(origin.x, origin.z, 0.0f);
		K::Vector3 D = C + direction;
		for (int i = 0; i < Physics::colliders.size(); i++)
		{
			if (K::Physics::IsInLayer(Physics::colliders[i], avoidLayer))
			{
				if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Line)
				{
					for (int j = 0; j < Physics::colliders[i]->GetNumberOfPoints(); j++)
					{
						K::Vector3 A = Physics::colliders[i]->GetLine(j)->point[0];
						K::Vector3 B = Physics::colliders[i]->GetLine(j)->point[1];
						float a = (-(A.x * B.y - B.x * A.y) * (D.x - C.x) + (C.x * D.y - D.x * C.y) * (B.x - A.x)) / (-(C.y - D.y) * (B.x - A.x) + (A.y - B.y) * (D.x - C.x));
						float b = (-(A.y - B.y) * a - (A.x * B.y - B.x * A.y)) / (B.x - A.x);
						float c = (-(C.y - D.y) * a - (C.x * D.y - D.x * C.y)) / (D.x - C.x);
						K::Vector3 J = K::Vector3(a, 0.0f, 0.0f);
						if (B.x - A.x == 0.0f)
						{
							J.y = c;
						}
						else 
						{
							J.y = b;
						}
						K::Vector3 AJ = (J - A).normalise();
						K::Vector3 BJ = (J - B).normalise();
						K::Vector3 CJ = (J - C).normalise();
						K::Vector3 DJ = (J - D).normalise();
						K::Vector3 BA = (A - B).normalise();
						K::Vector3 CD = (D - C).normalise();
						float U = K::Vector3::DotProduct(BJ, BA);
						float V = K::Vector3::DotProduct(AJ, BA);
						float W = K::Vector3::DotProduct(CD, CJ);
						float N = K::Vector3::DotProduct(DJ, CJ);
						if (U > 0.0f && V < 0.0f && W > 0.0f && N < 0.0f)
						{
							if(hit != nullptr)
								*hit = Physics::colliders[i];
							return true;
						}
					}
				}
				else if (Physics::colliders[i]->colliderType == K::Collider::ColliderType::Circle)
				{
					K::Vector3 J = *Physics::colliders[i]->PointOnLine(C, D, *Physics::colliders[i]->GetPosition());
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

	K::Vector3* Physics::GetCollisionResolution(K::Collider* col, std::vector<K::Layer> avoidLayer)
	{
		K::Vector3* offsetAmount = new K::Vector3();
		int count = 0;
		if (col->colliderType == K::Collider::ColliderType::Circle) 
		{
			K::Vector3 position = *col->GetPosition();
			for (K::ContactPoint J : K::Physics::GetClosestPoints(position, avoidLayer))
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
						col->other = J.other;
						count++;
					}
				}
			}
		}
		if (offsetAmount->magnitude() <= 0.0f || count == 0) 
		{
			col->SetIsColliding(false);
			col->other = nullptr;
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