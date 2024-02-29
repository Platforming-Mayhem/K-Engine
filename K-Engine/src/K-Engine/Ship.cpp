#include "Ship.h"

namespace K 
{
	Ship::Ship() 
	{

	}

	Ship::~Ship() 
	{

	}

	void Ship::Update() 
	{

	}

	void Ship::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Ship Settings")) 
		{

		}
	}

	void Ship::SetPropertyValues(const char* value, int valueIndex)
	{
		std::string temp = value;
		switch (valueIndex)
		{
		case 0:

			break;
		}
	}

	const char* Ship::GetPropertyValues()
	{
		return this->properties.c_str();
	}
	const char* Ship::GetName()
	{
		return typeid(K::Ship).name();
	}
}