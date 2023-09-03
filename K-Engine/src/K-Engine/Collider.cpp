#include "Collider.h"
#include "Time.h"
#include "InputManager.h"
#include "PhysicsManager.h"

namespace K 
{
	Collider::Collider() 
	{
		
	}

	Collider::~Collider()
	{
		PhysicsManager::Remove(this);
	}

	void Collider::Init()
	{
		this->id = std::chrono::system_clock::now().time_since_epoch().count();
		PhysicsManager::Attach(this);
	}

	int Collider::GetID() 
	{
		return this->id;
	}

	bool Collider::IsTriangleCollidingWithTriangleSAT(K::Collider* shape1, K::Collider* shape2) 
	{
		K::Collider* pointer1 = shape1;
		K::Collider* pointer2 = shape2;
		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				pointer1 = shape2;
				pointer2 = shape1;
			}
			for (int a = 0; a < pointer1->parent->GetMesh()->indices.size(); a++)
			{
				int b = (a + 1) % pointer1->parent->GetMesh()->indices.size();
				K::Vector3 tempA = K::Vector3(0.0f, 0.0f, 0.0f);
				K::Vector3 tempB = K::Vector3(0.0f, 0.0f, 0.0f);
				K::MultiplyMatrixVector(pointer1->parent->GetMesh()->vertices[pointer1->parent->GetMesh()->indices[a]].position, tempA, pointer1->parent->GetTransform()->modelMatrix);
				K::MultiplyMatrixVector(pointer1->parent->GetMesh()->vertices[pointer1->parent->GetMesh()->indices[b]].position, tempB, pointer1->parent->GetTransform()->modelMatrix);
				K::Vector3 axisProj = K::Vector3(-(tempB.z - tempA.z), 0.0f, tempB.x - tempA.x);
				axisProj.normalise();
				float minR1 = INFINITY, maxR1 = -INFINITY;
				for (int p : pointer1->parent->GetMesh()->indices)
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(pointer1->parent->GetMesh()->vertices[p].position, temp, pointer1->parent->GetTransform()->modelMatrix);
					float dot = K::Vector3::DotProduct(temp, axisProj);
					minR1 = min(minR1, dot);
					maxR1 = max(maxR1, dot);
				}
				float minR2 = INFINITY, maxR2 = -INFINITY;
				for (int p : pointer2->parent->GetMesh()->indices)
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(pointer2->parent->GetMesh()->vertices[p].position, temp, pointer2->parent->GetTransform()->modelMatrix);
					float dot = K::Vector3::DotProduct(temp, axisProj);
					minR2 = min(minR2, dot);
					maxR2 = max(maxR2, dot);
				}
				if (!(maxR2 >= minR1 && maxR1 >= minR2))
					return false;
			}
		}
		return true;
	}

	bool Collider::IsCollidingWithTriangleBarycentric(K::Vector3 P, float &u, float &v, float &w)
	{
		for (int i = 0; i < this->parent->GetMesh()->indices.size(); i++)
		{
			K::Vector3 Atemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 A = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(A, Atemp, this->parent->GetTransform()->modelMatrix);
			i++;
			K::Vector3 Btemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 B = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(B, Btemp, this->parent->GetTransform()->modelMatrix);
			i++;
			K::Vector3 Ctemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 C = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(C, Ctemp, this->parent->GetTransform()->modelMatrix);
			//Temp Values are the final values
			K::Vector3 v0 = Btemp - Atemp;
			K::Vector3 v1 = Ctemp - Atemp;
			K::Vector3 v2 = P - Atemp;
			float d00 = K::Vector3::DotProduct(v0, v0);
			float d01 = K::Vector3::DotProduct(v0, v1);
			float d11 = K::Vector3::DotProduct(v1, v1);
			float d20 = K::Vector3::DotProduct(v2, v0);
			float d21 = K::Vector3::DotProduct(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			v = (d11 * d20 - d01 * d21) / denom;
			w = (d00 * d21 - d01 * d20) / denom;
			u = 1.0f - v - w;
			return v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f;
		}
		return false;
	}

	bool Collider::IsCollidingWithTriangleAngle(K::Vector3 P)
	{
		//Check Desmos https://www.desmos.com/calculator/3c3j6yj6ld
		//https://youtu.be/4K-Jx914NcQ?t=724
		//https://www.youtube.com/watch?v=HYAgJN3x4GA
		//https://www.youtube.com/watch?v=3MJ-k15te_k&t=222s
		//NOTE(JAWAD): USE ANGLES, IF ANGLE IS GREATER THAN OR EQUAL TO 180 DEGREES THEN NOT COLLIDING ELSE IS COLLIDING
		for (int i = 0; i < this->parent->GetMesh()->indices.size(); i++)
		{
			K::Vector3 Atemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 A = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(A, Atemp, this->parent->GetTransform()->modelMatrix);
			i++;
			K::Vector3 Btemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 B = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(B, Btemp, this->parent->GetTransform()->modelMatrix);
			i++;
			K::Vector3 Ctemp = K::Vector3(0.0f, 0.0f, 0.0f);
			K::Vector3 C = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			K::MultiplyMatrixVector(C, Ctemp, this->parent->GetTransform()->modelMatrix);

			K::Vector3 AP = (P - Atemp);
			K::Vector3 BP = (P - Btemp);
			K::Vector3 CP = (P - Ctemp);

			float angle = K::Vector3::AngleBetweenVectors(AP, BP) + K::Vector3::AngleBetweenVectors(BP, CP) + K::Vector3::AngleBetweenVectors(CP, AP);

			if (roundf(angle * 100.0f) / 100.0f == 360.0f || roundf(angle * 100.0f) / 100.0f == 180.0f)
			{
				return true;
			}
		}
		return false;
	}

	bool Collider::IsCollidingTriangle() 
	{
		for (int a = 0; a < K::PhysicsManager::colliders.size(); a++) 
		{
			int b = (a + 1) % K::PhysicsManager::colliders.size();
			if (IsTriangleCollidingWithTriangleSAT(K::PhysicsManager::colliders[a], K::PhysicsManager::colliders[b]) && a != b)
			{
				return true;
			}
		}
		return false;
	}

	bool Collider::IsColliding()
	{
		for (K::Collider* col : K::PhysicsManager::colliders)
		{
			for (int i = 0; i < this->parent->GetMesh()->vertices.size(); i++)
			{
				if (this->GetID() != col->GetID())
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(this->parent->GetMesh()->vertices[i].position, temp, this->parent->GetTransform()->modelMatrix);
					float u, v, w;
					if (col->IsCollidingWithTriangleBarycentric(temp, u, v, w))
					{
						return true;
					}
				}
			}
			for (int j = 0; j < col->parent->GetMesh()->vertices.size(); j++)
			{
				if (col->GetID() != this->GetID())
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(col->parent->GetMesh()->vertices[j].position, temp, col->parent->GetTransform()->modelMatrix);
					float u, v, w;
					if (this->IsCollidingWithTriangleBarycentric(temp, u, v, w))
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void Collider::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Collider Settings")) 
		{
			ImGui::Checkbox("Is Colliding", &this->isColliding);
			ImGui::Checkbox("Is Static", &this->isStatic);
		}
	}

	void Collider::Update()
	{
		
	}

	void Collider::Unbind()
	{

	}

	void Collider::Bind()
	{
		
	}

	const char* Collider::GetName()
	{
		return typeid(K::Collider).name();
	}
}