#include "Fox.h"

namespace K 
{
	Fox::Fox() 
	{

	}

	Fox::~Fox() 
	{

	}

	void Fox::Update() 
	{
		this->AvoidFalling();
		this->AvoidWalls();
		this->Move();
		this->Gravity();
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