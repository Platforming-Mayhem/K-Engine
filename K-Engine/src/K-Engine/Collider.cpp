#include "Collider.h"
#include "Time.h"
#include "InputManager.h"
#include "Camera.h"
#include "PhysicsManager.h"

namespace K 
{
	Collider::Collider() 
	{
		
	}

	Collider::~Collider()
	{
		this->triangles.clear();
		PhysicsManager::Remove(this);
	}

	void Collider::Init()
	{
		this->id = std::chrono::system_clock::now().time_since_epoch().count();
		FormTriangles();
		PhysicsManager::Attach(this);
	}

	int Collider::GetID() 
	{
		return this->id;
	}

	bool Collider::IsCollidingWithTriangle(K::Vector3 P)
	{
		//Check Desmos https://www.desmos.com/calculator/3c3j6yj6ld
		//https://youtu.be/4K-Jx914NcQ?t=724
		//https://www.youtube.com/watch?v=HYAgJN3x4GA
		//https://www.youtube.com/watch?v=3MJ-k15te_k&t=222s
		//NOTE(JAWAD): USE ANGLES, IF ANGLE IS GREATER THAN OR EQUAL TO 180 DEGREES THEN NOT COLLIDING ELSE IS COLLIDING
		for (int i = 0; i < this->triangles.size(); i++) 
		{
			K::Vector3 A = this->triangles[i].vertices->data()[0];
			K::Vector3 B = this->triangles[i].vertices->data()[1];
			K::Vector3 C = this->triangles[i].vertices->data()[2];


			K::Vector3 AP = P - A;
			K::Vector3 BP = P - B;
			K::Vector3 CP = P - C;

			float angle = K::Vector3::AngleBetweenVectors(AP, BP) + K::Vector3::AngleBetweenVectors(BP, CP) + K::Vector3::AngleBetweenVectors(CP, AP);

			if (roundf(angle * 100.0f) / 100.0f == 360.0f || roundf(angle * 100.0f) / 100.0f == 180.0f)
			{
				return true;
			}
		}
		return false;
	}

	void Collider::FormTriangles() 
	{
		std::vector<K::Triangle> temp;
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

			temp.push_back(K::Triangle(Atemp, Btemp, Ctemp));
		}
		this->triangles = temp;
	}

	void Collider::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Collider Settings")) 
		{
			ImGui::Checkbox("Static", &this->isStatic);
			ImGui::Text("Triangles: %i", this->triangles.size());
			if (!this->isStatic) 
			{
				for (int i = 0; i < this->parent->GetMesh()->vertices.size(); i++)
				{
					K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
					K::MultiplyMatrixVector(this->parent->GetMesh()->vertices.data()[i].position, temp, this->parent->GetTransform()->modelMatrix);
					if (PhysicsManager::IsColliding(temp))
					{
						ImGui::Text("Colliding");
					}
				}
			}
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
		FormTriangles();
	}

	const char* Collider::GetName()
	{
		return typeid(K::Collider).name();
	}
}