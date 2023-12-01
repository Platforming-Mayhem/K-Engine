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

	void Player::Init() 
	{
		
	}

	void Player::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Player Settings")) 
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			ImGui::Checkbox("Has Gravity", &this->hasGravity);
		}
	}

	void Player::Update() 
	{
		K::Vector3* direction = new K::Vector3();
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			*direction += new K::Vector3(1.0f, 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT))
		{
			*direction += new K::Vector3(-1.0f, 0.0f, 0.0f);
		}
		if (direction->magnitude() > 0.0f) 
		{
			direction->normalise();
			*direction = *direction * (K::Time::deltaTime() * this->movementSpeed);
			*(this->parent->GetTransform()->position) += *direction;
		}
		if (!K::Physics::IsColliding(this->parent) && this->hasGravity)
		{
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, -K::Time::deltaTime());
		}
	}

	void Player::Unbind() 
	{
		if(!this->hasGravity)
			this->hasGravity = true;
	}

	void Player::Bind() 
	{
		
	}
}