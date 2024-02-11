#include "Fox.h"

namespace K 
{
	Fox::Fox() 
	{
		this->minAttackFrame = 10;
		this->maxAttackFrame = 12;
	}

	Fox::~Fox() 
	{

	}

	void Fox::Update() 
	{
		this->Attack();
		this->AvoidFalling();
		this->AvoidWalls();
		this->Move();
		this->JumpUpdate();
		this->Gravity();
		this->ChooseDirection();
	}

	void Fox::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Fox Settings"))
		{
			ImGui::DragFloat("Movement Speed", &this->movementSpeed);
			this->RaycastVisualiser();
		}
	}

	void Fox::SetPropertyValues(const char* value, int valueIndex)
	{
		std::string temp = value;
		switch (valueIndex)
		{
		case 0:
			this->movementSpeed = std::stof(temp);
			break;
		}
	}

	const char* Fox::GetPropertyValues()
	{
		this->properties = std::to_string(this->movementSpeed);
		return this->properties.c_str();
	}

	const char* Fox::GetName()
	{
		return typeid(K::Fox).name();
	}
}