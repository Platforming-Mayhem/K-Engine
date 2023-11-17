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

	void Player::SetPropertyValues(const char* value)
	{
		std::string temp = value;
		std::string a(1, temp.back());
		int propertyNumber = std::stoi(a);
		temp.pop_back();
		switch (propertyNumber)
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
		}
	}

	void Player::Update() 
	{
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(-K::Time::deltaTime() * this->movementSpeed, 0.0f, 0.0f);
		}
		if (InputManager::IsKeyPressed(GLFW_KEY_UP))
		{
			*(this->parent->GetTransform()->position) += new K::Vector3(0.0f, 0.0f, K::Time::deltaTime() * this->movementSpeed);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_DOWN))
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