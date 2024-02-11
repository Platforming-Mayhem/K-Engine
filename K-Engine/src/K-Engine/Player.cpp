#include "Player.h"
#include "InputManager.h"
#include "Time.h"
#include "GameObject.h"
#include "Editor.h"

namespace K 
{
	Player::Player()
	{
		
	}

	Player::~Player() 
	{
		this->animator = nullptr;
		this->col = nullptr;
		this->sprite = nullptr;
		delete this->direction;
		std::cout << "Player Destructor" << std::endl;
	}

	const char* Player::GetPropertyValues()
	{
		this->properties = std::to_string(this->movementSpeed);
		return this->properties.c_str();
	}

	const char* Player::GetName()
	{
		return typeid(K::Player).name();
	}

	float Player::GetMovementSpeed() 
	{
		return this->movementSpeed;
	}

	void Player::SetPropertyValues(const char* value, int valueIndex)
	{
		std::string temp = value;
		switch (valueIndex)
		{
		case 0:
			this->movementSpeed = std::stof(temp);
			break;
		}
	}

	float easeOutQuint(float time, float power)
	{
		return 1 - powf(1 - time, power);
	}

	float decelerateEaseOutQuint(float time, float power)
	{
		return 1 - powf(1 - time - 1, power);
	}

	void Player::Init() 
	{
		this->direction = new K::Vector3();
		if (this->parent->GetComponentOfType(typeid(K::Animator).name()) != nullptr)
		{
			this->animator = (K::Animator*)this->parent->GetComponentOfType(typeid(K::Animator).name());
		}
		if (this->parent->GetComponentOfType(typeid(K::Collider).name()) != nullptr)
		{
			this->col = (K::Collider*)this->parent->GetComponentOfType(typeid(K::Collider).name());
		}
		if (this->parent->GetComponentOfType(typeid(K::Sprite).name()) != nullptr)
		{
			this->sprite = (K::Sprite*)this->parent->GetComponentOfType(typeid(K::Sprite).name());
		}

		this->originalScale = *this->parent->GetTransform()->scale;

		this->parent->layer = (int)K::Layer::LayerType::Player;
		this->time = 0.0f;
	}

	void Player::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Player Settings")) 
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
		}
	}

	void Player::Update() 
	{
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			this->animator->PlayAnimation(1, this->sprite, false);
			*this->direction = K::Vector3(this->movementSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT))
		{
			this->animator->PlayAnimation(1, this->sprite, false);
			*this->direction = K::Vector3(-this->movementSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
		}
		else
		{
			this->animator->PlayAnimation(0, this->sprite, false);
			*this->direction = K::Vector3(0.0f, 0.0f, 0.0f);
		}

		*(this->parent->GetTransform()->position) += this->direction;

		if (this->isJumping)
		{
			this->animator->PlayAnimation(2, this->sprite, false);
			if (this->flip)
			{
				this->parent->GetTransform()->scale->x = -this->originalScale.x;
			}
			else
			{
				this->parent->GetTransform()->scale->x = this->originalScale.x;
			}
		}
		else
		{
			if (direction->x != 0.0f)
			{
				if (this->flip)
				{
					this->parent->GetTransform()->scale->x = -this->originalScale.x;
				}
				else 
				{
					this->parent->GetTransform()->scale->x = this->originalScale.x;
				}
			}
			else
			{
				if (this->flip)
				{
					this->parent->GetTransform()->scale->x = -this->originalScale.x;
				}
				else
				{
					this->parent->GetTransform()->scale->x = this->originalScale.x;
				}
			}
		}

		if (InputManager::IsKeyPressedDown(GLFW_KEY_UP) && this->jumpTime == 0.0f)
		{
			this->isJumping = true;
		}
		else if (InputManager::IsKeyReleased(GLFW_KEY_UP) && this->jumpTime < 1.0f)
		{
			this->jumpTime = 1.0f;
		}
		else if (K::Physics::IsColliding(this->parent) && this->jumpTime > 0.0f)
		{
			this->jumpTime = 0.0f;
			this->isJumping = false;
		}

		if (this->isJumping) 
		{
			this->time = 0.0f;
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, (-(this->jumpTime - 0.5f) + 0.5f) * 1.0f);
			this->jumpTime += K::Time::deltaTime() * 2.0f;
		}
		else 
		{
			if (!K::Physics::IsColliding(this->parent) && !K::Physics::IsStatic(this->parent)) 
			{
				*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, -this->time);
				this->time += K::Time::deltaTime();
			}
			else 
			{
				this->time = 0.0f;
			}
		}
	}

	void Player::Unbind() 
	{
		if (this->direction->x > K::Time::deltaTime()) 
		{
			this->flip = false;
		}
		else if(this->direction->x < -K::Time::deltaTime())
		{
			this->flip = true;
		}
	}

	void Player::Bind() 
	{
		
	}
}