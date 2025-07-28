#include "K-Engine/Component.h"

namespace K
{
	const char* GetTypeName(Component &comp)
	{
		#if __unix__
		int status;
		std::string demangledName = std::string("class ") + std::string(abi::__cxa_demangle(typeid(comp).name(), NULL, NULL, &status));
		// Dynamically allocate memory for the returned string
    	char* ptr = new char[demangledName.size() + 1]; // +1 for terminating NUL

    	// Copy source string in dynamically allocated string buffer
    	strcpy(ptr, demangledName.c_str());
		return ptr;
		
		#else
		return typeid(comp).name();
		#endif
	}

	void Component::SetParent(K::GameObject* newParent)
	{
		this->parent = newParent;
	}

	const char* Component::GetName() 
	{
		return GetTypeName(*this);
	}

	Component::~Component() 
	{
		this->parent = nullptr;
	}
}