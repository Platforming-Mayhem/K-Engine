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
		*this->parent->GetTransform()->position += K::Vector3(K::Time::deltaTime(), 0.0f, 0.0f);
	#endif
}

void K::Move::UpdateEditor()
{
	if (ImGui::CollapsingHeader("Move Settings")) 
	{

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

}

const char* K::Move::GetPropertyValues()
{
	this->properties = "";
	return this->properties.c_str();
}

const char* K::Move::GetName()
{
	return typeid(K::Move).name();
}
