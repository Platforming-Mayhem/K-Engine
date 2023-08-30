#pragma once
#include "build.h"
#include "Component.h"
#include "Transform.h"

namespace K
{
	struct Triangle 
	{
		std::vector<K::Vector3> vertices[3];
		K::Vector3 GetMidPoint() 
		{
			K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
			temp.x = (vertices->data()[0].x + vertices->data()[1].x + vertices->data()[2].x) / 3.0f;
			temp.y = (vertices->data()[0].y + vertices->data()[1].y + vertices->data()[2].y) / 3.0f;
			temp.z = (vertices->data()[0].z + vertices->data()[1].z + vertices->data()[2].z) / 3.0f;
			return temp;
		}

		float CalculateArea() 
		{
			K::Vector3 A = vertices->data()[0];
			K::Vector3 B = vertices->data()[1];
			K::Vector3 C = vertices->data()[2];
			float a = (B - A).magnitude();
			float b = (C - B).magnitude();
			float c = (C - A).magnitude();
			float s = (a + b + c) / 2.0f;
			float area = sqrtf(s * (s - a) * (s - b) * (s - c));
			return area;
		}

		Triangle(K::Vector3 a, K::Vector3 b, K::Vector3 c)
		{
			vertices->push_back(a);
			vertices->push_back(b);
			vertices->push_back(c);
		}
	};

	class K_API Collider : public K::Component
	{
	private:
		std::vector<K::Triangle> triangles;
		bool isStatic = true;
		int id;
	public:
		Collider();

		virtual ~Collider();

		bool IsColliding();

		bool IsCollidingWithTriangle(K::Vector3 P);

		int GetID();

		void FormTriangles();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		const char* GetName() override;
	};
}