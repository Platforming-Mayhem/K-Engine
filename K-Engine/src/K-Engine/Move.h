#pragma once
#include "build.h"
#include "Component.h"

namespace K 
{
	class K_API Move : public K::Component 
	{
	private:
		std::string properties;
	public:
		Move();

		~Move();

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