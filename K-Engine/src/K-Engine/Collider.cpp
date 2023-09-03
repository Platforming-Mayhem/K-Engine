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

	bool Collider::IsTriangleCollidingWithTriangleSAT(K::Triangle* shape1, K::Triangle* shape2)
	{
		K::Triangle* pointer1 = shape1;
		K::Triangle* pointer2 = shape2;
		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				pointer1 = shape2;
				pointer2 = shape1;
			}
			for (int a = 0; a < pointer1->vertices->size(); a++)
			{
				int b = (a + 1) % pointer1->vertices->size();
				K::Vector3 axisProj = K::Vector3(-(pointer1->vertices->data()[b].z - pointer1->vertices->data()[a].z), 0.0f, pointer1->vertices->data()[b].x - pointer1->vertices->data()[a].x).normalise();
				float minR1 = INFINITY, maxR1 = -INFINITY;
				for (int p = 0; p < pointer1->vertices->size(); p++)
				{
					float dot = K::Vector3::DotProduct(pointer1->vertices->data()[p], axisProj);
					minR1 = min(minR1, dot);
					maxR1 = max(maxR1, dot);
				}
				float minR2 = INFINITY, maxR2 = -INFINITY;
				for (int p = 0; p < pointer2->vertices->size(); p++)
				{
					float dot = K::Vector3::DotProduct(pointer2->vertices->data()[p], axisProj);
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
		for (K::Collider* col : K::PhysicsManager::colliders)
		{
			if (col->GetID() != this->GetID()) 
			{
				for (int a = 0; a < col->parent->GetMesh()->indices.size(); a++)
				{
					K::Vector3 Atemp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::Vector3 A = K::Vector3(col->parent->GetMesh()->vertices[col->parent->GetMesh()->indices[a]].position);
					K::MultiplyMatrixVector(A, Atemp, col->parent->GetTransform()->modelMatrix);
					a++;
					K::Vector3 Btemp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::Vector3 B = K::Vector3(col->parent->GetMesh()->vertices[col->parent->GetMesh()->indices[a]].position);
					K::MultiplyMatrixVector(B, Btemp, col->parent->GetTransform()->modelMatrix);
					a++;
					K::Vector3 Ctemp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::Vector3 C = K::Vector3(col->parent->GetMesh()->vertices[col->parent->GetMesh()->indices[a]].position);
					K::MultiplyMatrixVector(C, Ctemp, col->parent->GetTransform()->modelMatrix);

					K::Triangle tri1 = K::Triangle(Atemp, Btemp, Ctemp);

					for (int b = 0; b < this->parent->GetMesh()->indices.size(); b++)
					{
						A = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[b]].position);
						K::MultiplyMatrixVector(A, Atemp, this->parent->GetTransform()->modelMatrix);
						b++;
						B = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[b]].position);
						K::MultiplyMatrixVector(B, Btemp, this->parent->GetTransform()->modelMatrix);
						b++;
						C = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[b]].position);
						K::MultiplyMatrixVector(C, Ctemp, this->parent->GetTransform()->modelMatrix);

						K::Triangle tri2 = K::Triangle(Atemp, Btemp, Ctemp);

						if (IsTriangleCollidingWithTriangleSAT(&tri1, &tri2))
						{
							return true;
						}
					}
				}
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
		this->isColliding = IsCollidingTriangle();
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