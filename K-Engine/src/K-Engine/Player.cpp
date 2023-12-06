#include "Player.h"
#include "InputManager.h"
#include "Time.h"
#include "GameObject.h"

namespace K 
{
	Player::Player()
	{
		
	}

	Player::~Player() 
	{
		std::cout << "Player Destructor" << std::endl;
		delete this->direction;
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
		if (!K::Physics::IsStatic(this->parent))
		{
			if (K::Physics::IsColliding(this->parent))
			{
				this->time = 0.0f;
				this->jumpTime = 0.0f;
			}
			else
			{
				*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, -this->time * 0.2f);
				this->time += 1.0f / 120.0f;
			}
		}
		else
		{
			this->time = 0.0f;
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_SPACE) && this->jumpTime < 1.0f)
		{
			this->time = 0.0f;
			this->jumpTime += K::Time::deltaTime() * 2.0f;
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, (-(this->jumpTime - 0.5f) + 0.5f) * 0.1f);
		}
		else if (InputManager::IsKeyReleased(GLFW_KEY_SPACE) && !K::Physics::IsColliding(this->parent))
		{
			this->jumpTime = 1.0f;
		}
		float accelerationSpeed = (easeOutQuint(this->accelerationTime, 4.0f) * this->movementSpeed);
		float decelerationSpeed = (decelerateEaseOutQuint(this->decelerationTime, 2.0f) * this->movementSpeed);
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			if (this->accelerationTime < 1.0f)
			{
				this->accelerationTime += K::Time::deltaTime() * 0.5f;
			}
			else 
			{
				this->accelerationTime = 1.0f;
			}
			this->decelerationTime = 0.0f;
			this->direction = new K::Vector3(accelerationSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT))
		{
			if (this->accelerationTime < 1.0f)
			{
				this->accelerationTime += K::Time::deltaTime() * 0.5f;
			}
			else
			{
				this->accelerationTime = 1.0f;
			}
			this->decelerationTime = 0.0f;
			this->direction = new K::Vector3(-accelerationSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
		}
		else 
		{
			if (this->direction->x > 0.0f) 
			{
				if (this->decelerationTime < 1.0f)
				{
					this->decelerationTime += K::Time::deltaTime() * 4.0f;
				}
				else
				{
					this->decelerationTime = 1.0f;
				}
				this->direction = new K::Vector3(decelerationSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
			}
			else if (this->direction->x < 0.0f) 
			{
				if (this->decelerationTime < 1.0f)
				{
					this->decelerationTime += K::Time::deltaTime() * 4.0f;
				}
				else
				{
					this->decelerationTime = 1.0f;
				}
				this->direction = new K::Vector3(-decelerationSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
			}
			if (this->accelerationTime > 0.0f) 
			{
				this->decelerationTime = 1.0f - this->accelerationTime;
				this->accelerationTime = 0.0f;
			}
		}
		*(this->parent->GetTransform()->position) += this->direction;
	}

	void Player::Unbind() 
	{
		
	}

	void Player::Bind() 
	{
		
	}
}