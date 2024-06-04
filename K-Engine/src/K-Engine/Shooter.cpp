#include "Shooter.h"

namespace K 
{
	Shooter::Shooter() 
	{

	}

	Shooter::~Shooter()
	{
		for (K::Bullet* bullet : this->bullets)
		{
			delete bullet;
		}
		bullets.clear();
	}

	void Shooter::Init()
	{
		this->parent->layer = (int)K::Layer::LayerType::Enemy;
		if (this->parent->GetComponentOfType(typeid(K::Collider).name()) != nullptr)
		{
			this->col = (K::Collider*)this->parent->GetComponentOfType(typeid(K::Collider).name());
		}
	}

	void Shooter::Update()
	{
		if (Physics::Hitbox(*this->col->GetPosition() + K::Vector3(-this->col->GetRadius() - 2.0f, 0.0f, -this->col->GetRadius()), *this->col->GetPosition() + K::Vector3(-this->col->GetRadius(), 0.0f, this->col->GetRadius() + 1.0f), { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }) && this->reloadTime <= 0.0f) 
		{
			this->bullets.push_back(new K::Bullet(*this->col->GetPosition(), K::Vector3(-1.0f, 0.0f, 0.0f), 10.0f, 1.0f));
			this->reloadTime = 2.0f;
		}
		if (this->reloadTime > 0.0f) 
		{
			this->reloadTime -= K::Time::deltaTime();
		}
		if (!this->bullets.empty())
		{
			for (int i = 0; i < this->bullets.size(); i++)
			{
				if (this->bullets[i]->Update())
				{
					delete this->bullets[i];
					this->bullets.erase(this->bullets.begin() + i);
				}
			}
		}
	}

	void Shooter::Render() 
	{
		if (!bullets.empty())
		{
			for (K::Bullet* bullet : bullets)
			{
				K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
				*temp.position = bullet->GetLocation();
				temp.PassModelMatrix();
				glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);
				bullet->Render();
			}
		}
	}

	void Shooter::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Shooter Settings"))
		{
			ImGui::DragFloat("Projectile Speed", &this->projectileSpeed);
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
			K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
			temp.PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);
			glBegin(GL_LINE_STRIP);
			glVertex3f(this->col->GetPosition()->x - this->col->GetRadius() - 2.0f, this->col->GetPosition()->y, this->col->GetPosition()->z - this->col->GetRadius());
			glVertex3f(this->col->GetPosition()->x - this->col->GetRadius(), this->col->GetPosition()->y, this->col->GetPosition()->z + this->col->GetRadius() + 1.0f);
			glEnd();
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
		}
	}

	void Shooter::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				this->projectileSpeed = std::stof(temp);
				break;
			}
		}
	}

	const char* Shooter::GetPropertyValues()
	{
		this->properties = std::to_string(this->projectileSpeed);
		return this->properties.c_str();
	}

	const char* Shooter::GetName()
	{
		return typeid(K::Shooter).name();
	}
}