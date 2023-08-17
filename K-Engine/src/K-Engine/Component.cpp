#include "Component.h"

namespace K
{
	void Component::SetParent(K::GameObject* newParent)
	{
		this->parent = newParent;
	}
}