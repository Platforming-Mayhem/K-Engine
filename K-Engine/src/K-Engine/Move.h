#pragma once
#include "build.h"
#include "Time.h"
#include "Component.h"
#include "Transform.h"

namespace K 
{
	class K_API Move : public K::Component 
	{
	private:
		float moveSpeed = 1.0f;
		float time = 0.0f;
		K::Vector3 destination;
		std::string properties;
	public:
		Move();

		~Move();

		float EaseInQuart(float x);

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}