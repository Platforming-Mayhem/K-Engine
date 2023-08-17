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

	bool Collider::IsCollidingWithTriangle(K::Vector3 A, K::Vector3 B, K::Vector3 C, K::Vector2 P)
	{
		//Check Desmos https://www.desmos.com/calculator/3c3j6yj6ld
		//https://youtu.be/4K-Jx914NcQ?t=724
		//https://www.youtube.com/watch?v=HYAgJN3x4GA

		if (A.z == C.z)
			C.z -= 1/120;

		float s1 = C.z - A.z;
		float s2 = C.x - A.x;
		float s3 = B.z - A.z;
		float s4 = P.y - A.z;

		float w1 = (A.x * s1 + s4 * s2 - P.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
		float w2 = (s4 - w1 * s3) / s1;

		return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
	}

	bool Collider::IsCollidingAtPoint(K::Vector2 P)
	{
		for (int i = 0; i < this->triangles.size(); i++)
		{
			K::Vector3 out1 = K::Vector3(0.0f, 0.0f, 0.0f);
			MultiplyMatrixVector(this->triangles[i].vertices->data()[0], out1, this->parent->GetTransform()->modelMatrix);
			K::Vector3 out2 = K::Vector3(0.0f, 0.0f, 0.0f);
			MultiplyMatrixVector(this->triangles[i].vertices->data()[1], out2, this->parent->GetTransform()->modelMatrix);
			K::Vector3 out3 = K::Vector3(0.0f, 0.0f, 0.0f);
			MultiplyMatrixVector(this->triangles[i].vertices->data()[2], out3, this->parent->GetTransform()->modelMatrix);

			if (IsCollidingWithTriangle(out1, out2, out3, P))
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