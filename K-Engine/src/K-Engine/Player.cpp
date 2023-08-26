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
			for (int i = 0; i < this->parent->GetMesh()->vertices.size(); i++)
			{
				K::Vector3 temp = K::Vector3(0.0f, 0.0f, 0.0f);
				K::MultiplyMatrixVector(this->parent->GetMesh()->vertices.data()[i].position, temp, this->parent->GetTransform()->modelMatrix);
				if (PhysicsManager::IsColliding(temp))
				{
					ImGui::Text("Colliding");
				}
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

	const char* Player::GetName()
	{
		return typeid(K::Player).name();
	}
}