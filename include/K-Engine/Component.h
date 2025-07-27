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

		std::string GetName();

		virtual const char* GetPropertyValues() = 0;

		virtual void SetPropertyValues(const char* value, int valueIndex) = 0;

		void SetParent(K::GameObject* newParent);
	};
}