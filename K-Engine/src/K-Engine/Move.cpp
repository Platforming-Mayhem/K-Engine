#include "Move.h"
#include "Time.h"
#include "GameObject.h"

K::Move::Move()
{
	
}

K::Move::~Move()
{

}

float K::Move::EaseInQuart(float x) 
{
	return x * x * x * x;
}

void K::Move::Init()
{
	K::Time::startTime = 0.0f;
	K::Time::endTime = 0.0f;
	this->time = 0.0f;
}

void K::Move::Update()
{
	#if _DEBUG
		
	#else
		float speed = EaseInQuart(this->time);
		if (speed < 1.0f)
		{
			this->time += K::Time::deltaTime();
		}
		else if(speed >= 1.0f)
		{
			this->time = 1.0f;
		}
		this->parent->GetTransform()->position->x += speed * K::Time::deltaTime() * this->moveSpeed;
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
