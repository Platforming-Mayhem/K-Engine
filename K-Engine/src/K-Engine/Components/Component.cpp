#include "K-Engine/Component.h"

namespace K
{
	void Component::SetParent(K::GameObject* newParent)
	{
		this->parent = newParent;
	}

	const char* Component::GetName() 
	{
		return typeid(*this).name();
	}

	Component::~Component() 
	{
		this->parent = nullptr;
	}
}