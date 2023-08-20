#include "Player.h"
#include "InputManager.h"
#include "Camera.h"
#include "Time.h"
#include "PhysicsManager.h"

namespace K 
{
	Player::Player()
	{
		
	}

	Player::~Player() 
	{
		
	}

	void Player::Init() 
	{
		
	}

	void Player::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Player Settings")) 
		{
			if (PhysicsManager::IsColliding(K::Vector3(this->parent->GetTransform()->position->x, this->parent->GetTransform()->position->y, this->parent->GetTransform()->position->z)))
			{
				ImGui::Text("Colliding");
			}
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
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
	}

	void Player::Unbind() 
	{
		
	}

	void Player::Bind() 
	{
		
	}

	const char* Player::GetName()
	{
		return typeid(K::Player).name();
	}
}