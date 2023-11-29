#pragma once
#include "build.h"
#include "Component.h"
#include "GameObject.h"

namespace K
{
	struct Line 
	{
		K::Vector3 point[2];
		Line(K::Vector3 v1, K::Vector3 v2) 
		{
			this->point[0] = v1;
			this->point[1] = v2;
		}
	};

	class K_API Collider : public K::Component
	{
	private:
		bool isStatic = true;
		K::Vector3* pointOnLine;
		float radius = 1.0f;
		int selectedLine = 0;
		std::vector<K::Line> linePoints;
		std::string properties;
	public:
		enum class ColliderType { Circle = 0, Line = 1 };

		ColliderType colliderType = ColliderType::Circle;

		Collider();

		~Collider();

		void CircleEditor();

		void CircleColliderStatic();

		void CircleCollider();

		void LineEditor();

		K::Vector3* PointOnLine(K::Vector3 A, K::Vector3 B, K::Vector3 P);

		K::Vector3* ClosestPointLineCollider(K::Vector3 P);

		int GetNumberOfPoints();

		void LineColliderStatic();

		void LineCollider();

		void LineVisualDebug();

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