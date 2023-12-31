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
		this->parent->layer = (int)K::Layer::LayerType::Enemy;
		this->time = 0.0f;
		if (this->parent->GetComponentOfType(typeid(K::Animator).name()) != nullptr)
		{
			this->animator = (K::Animator*)this->parent->GetComponentOfType(typeid(K::Animator).name());
		}
		if (this->parent->GetComponentOfType(typeid(K::Collider).name()) != nullptr)
		{
			this->col = (K::Collider*)this->parent->GetComponentOfType(typeid(K::Collider).name());
		}
		this->sprite = (K::Sprite*)this->parent->GetComponentOfType(typeid(K::Sprite).name());
	}

	void Enemy::Update() 
	{
		if (Physics::Raycast(*this->col->GetPosition(), K::Vector3(1.0f, 0.0f, 0.0f), {K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player)}))
		{
			this->direction = K::Vector3(-K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		else if (Physics::Raycast(*this->col->GetPosition(), K::Vector3(-1.0f, 0.0f, 0.0f), { K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player) }))
		{
			this->direction = K::Vector3(K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		if (this->direction.x > 0.0f) 
		{
			this->animator->PlayAnimation(0, this->sprite);
		}
		else 
		{
			this->animator->PlayAnimation(1, this->sprite);
		}
		*this->parent->GetTransform()->position += this->direction;
		if (K::Physics::IsColliding(this->parent))
		{
			this->time = 0.0f;
		}
		else
		{
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, -this->time * 0.2f);
			this->time += K::Time::deltaTime();
		}
	}

	void Enemy::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Enemy Settings")) 
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
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
		std::string temp = value;
		switch (valueIndex)
		{
		case 0:
			this->movementSpeed = std::stof(temp);
			break;
		}
	}

	const char* Enemy::GetPropertyValues() 
	{
		this->properties = std::to_string(this->movementSpeed);
		return this->properties.c_str();
	}

	const char* Enemy::GetName() 
	{
		return typeid(K::Enemy).name();
	}
}