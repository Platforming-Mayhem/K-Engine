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
		FormTriangles();
		PhysicsManager::Attach(this);
	}

	bool Collider::IsCollidingWithTriangle(K::Vector3 P)
	{
		//Check Desmos https://www.desmos.com/calculator/3c3j6yj6ld
		//https://youtu.be/4K-Jx914NcQ?t=724
		//https://www.youtube.com/watch?v=HYAgJN3x4GA
		//K::Vector3 AB = B - A;
		//K::Vector3 AC = C - A;
		//K::Vector3 BC = C - B;
		//K::Vector3 Center = K::Vector3((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
		/*if (A.z == C.z)
			C.z -= 1 / 120;
		K::Triangle tri = K::Triangle(A, B, C);
		K::Triangle tri1 = K::Triangle(A, P, C);
		K::Triangle tri2 = K::Triangle(C, P, B);
		K::Triangle tri3 = K::Triangle(B, P, A);
		float collatedArea = tri1.CalculateArea() + tri2.CalculateArea() + tri3.CalculateArea();

		return collatedArea == tri.CalculateArea();*/
		/*if (A.z == C.z)
			C.z -= 1 / 120;

		float s1 = C.z - A.z;
		float s2 = C.x - A.x;
		float s3 = B.z - A.z;
		float s4 = P.z - A.z;

		float w1 = (A.x * s1 + s4 * s2 - P.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
		float w2 = (s4 - w1 * s3) / s1;

		return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;*/
		/*K::Triangle tri = K::Triangle(A, B, C);
		K::Vector3 AB = B - A;
		K::Vector3 AC = C - A;
		K::Vector3 AP = point - A;
		K::Vector3 normal = K::Vector3::CrossProduct(AB, AC).normalise();
		float distanceFromTri = K::Vector3::DotProduct(AP, normal);
		K::Vector3 P = AP - (normal * distanceFromTri);
		AP = P - A;
		K::Vector3 CP = P - C;
		K::Vector3 BP = P - B;
		K::Vector3 BC = C - B;
		K::Triangle triA = K::Triangle(AC, CP, AP), triB = K::Triangle(AP, BP, AB), triC = K::Triangle(BC, CP, BP);
		float collatedArea = triA.CalculateArea() + triB.CalculateArea() + triC.CalculateArea();
		if (collatedArea > tri.CalculateArea()) 
		{
			return false;
		}
		else 
		{
			return true;
		}*/
		//std::cout << POnTri.x << " " << POnTri.y << " " << POnTri.z << std::endl;
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

			//ImGui::Text("%f, %f, %f", K::Vector3::AngleBetweenVectors(AP, BP), K::Vector3::AngleBetweenVectors(BP, CP), K::Vector3::AngleBetweenVectors(CP, AP));

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
			K::Vector3 A = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			i++;
			K::Vector3 B = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);
			i++;
			K::Vector3 C = K::Vector3(this->parent->GetMesh()->vertices[this->parent->GetMesh()->indices[i]].position);

			temp.push_back(K::Triangle(A, B, C));
		}
		this->triangles = temp;
	}

	void Collider::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Collider Settings")) 
		{
			ImGui::Text("Triangles: %i", this->triangles.size());
		}
	}

	void Collider::Update()
	{
		if (this->parent->meshUpdate) 
		{
			FormTriangles();
			this->parent->meshUpdate = false;
		}
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