#pragma once
#include "K-Engine/build.h"

namespace K 
{
	class K_API GameObject;

	class K_API Component
	{
	public:
		K::GameObject* parent = nullptr;

		virtual ~Component();

		virtual void Init() {};

		virtual void UpdateEditor() = 0;

		virtual void Update() {};

		virtual void RenderInit() {};

		virtual void RenderBind() {};

		virtual void Render() {};

		virtual void RenderUnbind() {};

		virtual void Bind() {};

		virtual void Unbind() {};

		const char* GetName();

		virtual const char* GetPropertyValues() = 0;

		virtual void SetPropertyValues(const char* value, int valueIndex) = 0;

		void SetParent(K::GameObject* newParent);
	};

	const char* GetTypeName(Component &comp);

	template<typename T> const char* GetTypeName()
	{
		#if __unix__
		int status;
		std::string demangledName = std::string("class ") + std::string(abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status));
		// Dynamically allocate memory for the returned string
    	char* ptr = new char[demangledName.size() + 1]; // +1 for terminating NUL

    	// Copy source string in dynamically allocated string buffer
    	strcpy(ptr, demangledName.c_str());
		return ptr;
		
		#else
		return typeid(T).name();
		#endif
	}
}