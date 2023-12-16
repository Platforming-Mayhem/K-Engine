#include "Enemy.h"
#include "Time.h"

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
		if (Physics::Raycast(*this->parent->GetTransform()->position, K::Vector3(1.0f, 0.0f, 0.0f))) 
		{
			this->direction = K::Vector3(-K::Time::deltaTime(), 0.0f, 0.0f);
			std::cout << "Go Left" << std::endl;
		}
		else if (Physics::Raycast(*this->parent->GetTransform()->position, K::Vector3(-1.0f, 0.0f, 0.0f)))
		{
			this->direction = K::Vector3(K::Time::deltaTime(), 0.0f, 0.0f);
			std::cout << "Go Right" << std::endl;
		}
		if (K::Physics::IsColliding(this->parent))
		{
			this->time = 0.0f;
		}
		else
		{
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, -this->time * 0.2f);
			this->time += K::Time::deltaTime();
		}
		*this->parent->GetTransform()->position += this->direction;
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