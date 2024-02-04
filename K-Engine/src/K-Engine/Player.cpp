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
		for (int i = 0; i < K::Editor::GetCurrentScene()->GetNumberOfObjects(); i++)
		{
			K::GameObject* temp = K::Editor::GetCurrentScene()->GetGameObjects()[i];
			if (temp != this->parent) 
			{
				if (temp->GetComponentOfType(typeid(K::Camera).name()) != nullptr)
				{
					K::Camera* cam = (K::Camera*)temp->GetComponentOfType(typeid(K::Camera).name());
					cam->player = nullptr;
				}
			}
		}
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

		this->parent->layer = (int)K::Layer::LayerType::Player;
		this->time = 0.0f;
	}

	void Player::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Player Settings")) 
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			//this->HitboxVisualDebug();
		}
	}

	/*void Player::HitboxVisualDebug()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
		K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
		temp.PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);
		glBegin(GL_LINE_LOOP);
		glVertex3f(this->col->GetPosition()->x + 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z + 1.0f);//Top-Right
		glVertex3f(this->col->GetPosition()->x + 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z);//Bottom-Right
		glVertex3f(this->col->GetPosition()->x, this->col->GetPosition()->y, this->col->GetPosition()->z);//Bottom-Left
		glVertex3f(this->col->GetPosition()->x, this->col->GetPosition()->y, this->col->GetPosition()->z + 1.0f);//Top-Left
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex3f(this->col->GetPosition()->x, this->col->GetPosition()->y, this->col->GetPosition()->z + 1.0f);//Top-Right
		glVertex3f(this->col->GetPosition()->x, this->col->GetPosition()->y, this->col->GetPosition()->z);//Bottom-Right
		glVertex3f(this->col->GetPosition()->x - 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z);//Bottom-Left
		glVertex3f(this->col->GetPosition()->x - 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z + 1.0f);//Top-Left
		glEnd();
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
	}*/

	void Player::Update() 
	{
		float accelerationSpeed = (easeOutQuint(this->accelerationTime, 4.0f) * this->movementSpeed);
		float decelerationSpeed = (decelerateEaseOutQuint(this->decelerationTime, 2.0f) * this->movementSpeed);
		if (InputManager::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			if (this->accelerationTime < 1.0f)
			{
				this->accelerationTime += K::Time::deltaTime();
			}
			else
			{
				this->accelerationTime = 1.0f;
			}
			this->decelerationTime = 0.0f;
			this->attackDirection = 1.0f;
			this->direction = new K::Vector3(accelerationSpeed * K::Time::deltaTime(), 0.0f, 0.0f);
		}
		else if (InputManager::IsKeyPressed(GLFW_KEY_LEFT))
		{
			if (this->accelerationTime < 1.0f)
			{
				this->accelerationTime += K::Time::deltaTime();
			}
			else
			{
				this->accelerationTime = 1.0f;
			}
			this->decelerationTime = 0.0f;
			this->attackDirection = -1.0f;
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

		if (this->isJumping)
		{
			this->animator->PlayAnimation(2, this->sprite, false);
			if (this->flip)
			{
				this->parent->GetTransform()->scale->x = -this->sprite->GetTexture()->GetWidth() / 32.0f;
				this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
			}
			else
			{
				this->parent->GetTransform()->scale->x = this->sprite->GetTexture()->GetWidth() / 32.0f;
				this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
			}
		}
		else
		{
			if (direction->x != 0.0f)
			{
				this->animator->PlayAnimation(1, this->sprite, false);
				if (this->flip)
				{
					this->parent->GetTransform()->scale->x = -this->sprite->GetTexture()->GetWidth() / 32.0f;
					this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
				}
				else 
				{
					this->parent->GetTransform()->scale->x = this->sprite->GetTexture()->GetWidth() / 32.0f;
					this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
				}
			}
			else
			{
				this->animator->PlayAnimation(0, this->sprite, false);
				if (this->flip)
				{
					this->parent->GetTransform()->scale->x = -this->sprite->GetTexture()->GetWidth() / 32.0f;
					this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
				}
				else
				{
					this->parent->GetTransform()->scale->x = this->sprite->GetTexture()->GetWidth() / 32.0f;
					this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
				}
			}
		}

		if (InputManager::IsKeyPressedDown(GLFW_KEY_SPACE) && this->jumpTime == 0.0f)
		{
			this->isJumping = true;
		}
		else if (InputManager::IsKeyReleased(GLFW_KEY_SPACE) && this->jumpTime < 1.0f)
		{
			this->jumpTime = 1.0f;
		}
		else if (K::Physics::IsColliding(this->parent) && this->jumpTime >= 1.0f)
		{
			this->jumpTime = 0.0f;
			this->isJumping = false;
		}

		if (this->isJumping) 
		{
			this->time = 0.0f;
			*(this->parent->GetTransform()->position) += K::Vector3(0.0f, 0.0f, (-(this->jumpTime - 0.5f) + 0.5f) * 0.5f);
			this->jumpTime += K::Time::deltaTime() * 4.0f;
		}
		else 
		{
			if (!K::Physics::IsColliding(this->parent)) 
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