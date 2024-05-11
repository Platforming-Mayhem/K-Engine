#pragma once
#include "Enemy.h"

namespace K 
{
	class K_API Ship : public Enemy 
	{
	private:
		std::string properties;
	public:

		Ship();

		~Ship();

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;
	};
}