#include "Enemy.h"

namespace K 
{
	Enemy::Enemy() 
	{

	}

	Enemy::~Enemy() 
	{
		std::cout << "Enemy Destructor" << std::endl;
	}

	void Enemy::Init() 
	{

	}

	void Enemy::Update() 
	{
		
	}

	void Enemy::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Enemy Settings")) 
		{

		}
	}

	void Enemy::Bind() 
	{

	}

	void Enemy::Unbind()
	{

	}

	void Enemy::SetPropertyValues(const char* value, int valueIndex) 
	{

	}

	const char* Enemy::GetPropertyValues() 
	{
		return this->properties.c_str();
	}

	const char* Enemy::GetName() 
	{
		return typeid(K::Enemy).name();
	}
}