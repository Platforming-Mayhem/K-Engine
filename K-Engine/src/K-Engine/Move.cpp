#include "Move.h"
#include "Time.h"
#include "GameObject.h"

K::Move::Move()
{

}

K::Move::~Move()
{

}

void K::Move::Init()
{

}

void K::Move::Update()
{
	#if _DEBUG
		
	#else
		*this->parent->GetTransform()->position += K::Vector3(K::Time::deltaTime() * this->moveSpeed, 0.0f, 0.0f);
	#endif
}

void K::Move::UpdateEditor()
{
	if (ImGui::CollapsingHeader("Move Settings")) 
	{
		ImGui::DragFloat("Move Speed", &this->moveSpeed);
	}
}

void K::Move::Bind()
{

}

void K::Move::Unbind()
{

}

void K::Move::SetPropertyValues(const char* value, int valueIndex)
{
	if (value[0] != '\0' && value != nullptr)
	{
		std::string temp = value;
		switch (valueIndex)
		{
		case 0:
			this->moveSpeed = std::stof(temp);
			break;
		}
	}
}

const char* K::Move::GetPropertyValues()
{
	this->properties = std::to_string(this->moveSpeed);
	return this->properties.c_str();
}

const char* K::Move::GetName()
{
	return typeid(K::Move).name();
}
