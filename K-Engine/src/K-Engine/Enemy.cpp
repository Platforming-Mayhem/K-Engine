#include "Enemy.h"
#include "Time.h"
#include "Editor.h"

namespace K 
{
	Enemy::Enemy() 
	{
		
	}

	Enemy::~Enemy() 
	{
		this->animator = nullptr;
		this->col = nullptr;
		this->sprite = nullptr;
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
		this->direction = K::Vector3(-this->movementSpeed, 0.0f, 0.0f);
	}

	void Enemy::Attack() 
	{
		K::Collider* temp = nullptr;
		if (this->direction.x > 0.0f) 
		{
			if (Physics::Hitbox(*this->col->GetPosition() + K::Vector3(this->col->GetRadius(), 0.0f, -this->col->GetRadius()), *this->col->GetPosition() + K::Vector3(this->col->GetRadius() + 2.0f, 0.0f, this->col->GetRadius() + 1.0f), { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }, &temp))
			{
				if (this->animator != nullptr)
				{
					this->animator->PlayAnimation(2, this->sprite, false);
					this->isAttacking = true;
					if (this->sprite->GetFrameNumber() >= this->minAttackFrame && this->sprite->GetFrameNumber() <= this->maxAttackFrame && temp != nullptr)
					{
						K::Editor::Delete(temp->parent);
						temp = nullptr;
					}
				}
			}
		}
		else 
		{
			if (Physics::Hitbox(*this->col->GetPosition() + K::Vector3(-this->col->GetRadius() - 2.0f, 0.0f, -this->col->GetRadius()), *this->col->GetPosition() + K::Vector3(-this->col->GetRadius(), 0.0f, this->col->GetRadius() + 1.0f), { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }, &temp))
			{
				if (this->animator != nullptr)
				{
					this->animator->PlayAnimation(2, this->sprite, false);
					this->isAttacking = true;
					if (this->sprite->GetFrameNumber() >= this->minAttackFrame && this->sprite->GetFrameNumber() <= this->maxAttackFrame && temp != nullptr)
					{
						K::Editor::Delete(temp->parent);
						temp = nullptr;
					}
				}
			}
		}
		if (this->isAttacking && !this->sprite->IsPlaying()) 
		{
			this->isAttacking = false;
		}
	}

	void Enemy::AvoidFalling() 
	{
		if (!Physics::Raycast(*this->col->GetPosition() + K::Vector3(this->col->GetRadius(), 0.0f, 0.0f), K::Vector3(1.0f, -(this->col->GetRadius() + 0.1f), 0.0f), { K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player) }))
		{
			std::cout << "Avoid Falling: Move Left" << std::endl;
			this->direction = K::Vector3(-this->movementSpeed, 0.0f, 0.0f);
		}
		else if (!Physics::Raycast(*this->col->GetPosition() - K::Vector3(this->col->GetRadius(), 0.0f, 0.0f), K::Vector3(-1.0f, -(this->col->GetRadius() + 0.1f), 0.0f), { K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player) }))
		{
			std::cout << "Avoid Falling: Move Right" << std::endl;
			this->direction = K::Vector3(this->movementSpeed, 0.0f, 0.0f);
		}
	}

	void Enemy::AvoidWalls() 
	{
		if (Physics::Raycast(*this->col->GetPosition() + K::Vector3(this->col->GetRadius(), 0.0f, 0.0f), K::Vector3(1.0f, 0.0f, 0.0f), { K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player) }))
		{
			std::cout << "Avoid Walls: Move Left" << std::endl;
			this->direction = K::Vector3(-this->movementSpeed, 0.0f, 0.0f);
		}
		else if (Physics::Raycast(*this->col->GetPosition() - K::Vector3(this->col->GetRadius(), 0.0f, 0.0f), K::Vector3(-1.0f, 0.0f, 0.0f), { K::Layer(K::Layer::LayerType::Enemy), K::Layer(K::Layer::LayerType::Player) }))
		{
			std::cout << "Avoid Walls: Move Right" << std::endl;
			this->direction = K::Vector3(this->movementSpeed, 0.0f, 0.0f);
		}
	}

	void Enemy::Move() 
	{
		if (!this->isAttacking) 
		{
			if (this->animator != nullptr)
			{
				if (this->direction.x > 0.0f)
				{
					this->animator->PlayAnimation(1, this->sprite, false);
					this->parent->GetTransform()->scale->x = -this->sprite->GetTexture()->GetWidth() / 32.0f;
				}
				else
				{
					this->animator->PlayAnimation(1, this->sprite, false);
					this->parent->GetTransform()->scale->x = this->sprite->GetTexture()->GetWidth() / 32.0f;
				}
				this->parent->GetTransform()->scale->z = this->sprite->GetTexture()->GetHeight() / 32.0f;
			}
			*this->parent->GetTransform()->position += this->direction * K::Time::deltaTime();
		}
	}

	void Enemy::Gravity() 
	{
		if (K::Physics::IsColliding(this->parent))
		{
			this->time = 0.0f;
		}
		else
		{
			*this->parent->GetTransform()->position += K::Vector3(0.0f, 0.0f, -this->time);
			this->time += K::Time::deltaTime();
		}
	}

	void Enemy::Jump() 
	{
		
	}

	void Enemy::Update() 
	{
		
	}

	void Enemy::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Enemy Settings")) 
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			this->RaycastVisualiser();
		}
	}

	void Enemy::RaycastVisualiser() 
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
		K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
		temp.PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);
		//Avoid Falling
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z);
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius() + 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z - (this->col->GetRadius() + 0.1f));
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z);
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius() - 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z - (this->col->GetRadius() + 0.1f));
		glEnd();
		//Avoid Walls
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z);
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius() + 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z);
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius() - 1.0f, this->col->GetPosition()->y, this->col->GetPosition()->z);
		glEnd();
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 0.0f, 1.0f);
		//Attack Hitbox
		//RIGHT
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z - this->col->GetRadius());
		glVertex3f(this->col->GetPosition()->x + this->col->GetRadius() + 2.0f, this->col->GetPosition()->y, this->col->GetPosition()->z + this->col->GetRadius() + 1.0f);
		glEnd();
		//LEFT
		glBegin(GL_LINE_STRIP);
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius() - 2.0f, this->col->GetPosition()->y, this->col->GetPosition()->z - this->col->GetRadius());
		glVertex3f(this->col->GetPosition()->x - this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z + this->col->GetRadius() + 1.0f);
		glEnd();
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
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