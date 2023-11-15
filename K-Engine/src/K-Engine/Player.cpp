#include "Player.h"
#include "InputManager.h"
#include "Camera.h"
#include "Time.h"

namespace K 
{
	Player::Player()
	{
		this->properties = std::to_string(GetMovementSpeed());
	}

	Player::~Player() 
	{
		
	}

	const char* Player::GetPropertyValues()
	{
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

	void Player::SetPropertyValues(const char* value)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			this->movementSpeed = std::stof(value);
		}
	}

	void Player::Init() 
	{
		
	}

	void Player::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Player Settings")) 
		{
			if (ImGui::DragFloat("Movement Speed", &this->movementSpeed)) 
			{
				this->properties = std::to_string(GetMovementSpeed());
			}
		}
	}

	void Player::Update() 
	{
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT, window->window))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT, window->window))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(-K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_UP, window->window))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(0.0f, 0.0f, K::Time::deltaTime() * this->movementSpeed);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_DOWN, window->window))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(0.0f, 0.0f, -K::Time::deltaTime() * this->movementSpeed);
		}
	}

	void Player::Unbind() 
	{
		
	}

	void Player::Bind() 
	{
		
	}
}