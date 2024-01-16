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
		this->Gravity();
		this->Move();
	}

	void Fox::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Fox Settings"))
		{
			this->RaycastVisualiser();
		}
	}

	void Fox::SetPropertyValues(const char* value, int valueIndex)
	{
		
	}

	const char* Fox::GetPropertyValues()
	{
		return this->properties.c_str();
	}

	const char* Fox::GetName()
	{
		return typeid(K::Fox).name();
	}
}