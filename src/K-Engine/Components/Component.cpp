#include "K-Engine/Component.h"

namespace K
{
	void Component::SetParent(K::GameObject* newParent)
	{
		this->parent = newParent;
	}

	const char* Component::GetName() 
	{
		#if __unix__
		int status;
		std::string tempName = typeid(*this).name();
		std::string demangledName = std::string("class ") + abi::__cxa_demangle(tempName.c_str(), NULL, NULL, &status);
		return demangledName.c_str();
		
		#else
		return typeid(*this).name();
		#endif
	}

	Component::~Component() 
	{
		this->parent = nullptr;
	}
}