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
		K::Collider* hit = nullptr;
		if (Physics::HitCircle(*this->col->GetPosition(), this->radius, { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }, &hit) && this->reloadTime <= 0.0f)
		{
			K::Vector3 direction = (*hit->GetPosition() - *this->col->GetPosition());
			this->bullets.push_back(new K::Bullet(*this->col->GetPosition(), direction, 10.0f, 1.0f, this->projectileSpeed));
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

			ImGui::DragFloat("Radius", &this->radius);

			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
			K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
			*temp.position = *this->col->GetPosition();
			temp.PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);

			float theta = 360.0f / 16.0f;
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < 16; i++)
			{
				glVertex3f(this->radius * cosf((i * theta) / 57.2958f), 0.0f, this->radius * sinf((i * theta) / 57.2958f));
			}
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
			case 1:
				this->radius = std::stof(temp);
				break;
			}
		}
	}

	const char* Shooter::GetPropertyValues()
	{
		this->properties = std::to_string(this->projectileSpeed);
		this->properties += "," + std::to_string(this->radius);
		return this->properties.c_str();
	}

	const char* Shooter::GetName()
	{
		return typeid(K::Shooter).name();
	}
}