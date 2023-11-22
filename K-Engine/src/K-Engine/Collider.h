#pragma once
#include "build.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

namespace K
{
	struct Triangle 
	{
		K::Vector3* vertices[3];
		K::Vector3 GetMidPoint() 
		{
			K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
			temp.x = ((*this->vertices[0]).x + (*this->vertices[1]).x + (*this->vertices[2]).x) / 3.0f;
			temp.y = ((*this->vertices[0]).y + (*this->vertices[1]).y + (*this->vertices[2]).y) / 3.0f;
			temp.z = ((*this->vertices[0]).z + (*this->vertices[1]).z + (*this->vertices[2]).z) / 3.0f;
			return temp;
		}

		float CalculateArea() 
		{
			float a = (*this->vertices[1] - *this->vertices[0]).magnitude();
			float b = (*this->vertices[2] - *this->vertices[1]).magnitude();
			float c = (*this->vertices[2] - *this->vertices[0]).magnitude();
			float s = (a + b + c) / 2.0f;
			float area = sqrtf(s * (s - a) * (s - b) * (s - c));
			return area;
		}

		Triangle(K::Vector3* a, K::Vector3* b, K::Vector3* c)
		{
			this->vertices[0] = a;
			this->vertices[1] = b;
			this->vertices[2] = c;
		}
	};

	class K_API Collider : public K::Component
	{
	private:
		bool isStatic = true;
		std::string properties;
	public:
		Collider();

		~Collider();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value) override;

		void SetStaticState(bool state);

		const char* GetName() override;

		const char* GetPropertyValues() override;
	};
}