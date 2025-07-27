#include "K-Engine/Component.h"

namespace K
{
	void Component::SetParent(K::GameObject* newParent)
	{
		this->parent = newParent;
	}

	std::string Component::GetName() 
	{
		#if __unix__
		int status;
		std::string demangledName = std::string("class ") + std::string(abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, &status));
		return demangledName;
		
		#else
		return std::string(typeid(*this).name());
		#endif
	}

	Component::~Component() 
	{
		this->parent = nullptr;
	}
}