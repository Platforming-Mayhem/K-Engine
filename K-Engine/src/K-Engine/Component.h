#pragma once
#include "build.h"

namespace K 
{
	class K_API GameObject;

	class K_API Component
	{
	public:
		K::GameObject* parent = nullptr;

		virtual ~Component();

		virtual void Init() = 0;

		virtual void UpdateEditor() = 0;

		virtual void Update() = 0;

		virtual void RenderBind() {};

		virtual void Render() {};

		virtual void RenderUnbind() {};

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual const char* GetName() = 0;

		virtual const char* GetPropertyValues() = 0;

		virtual void SetPropertyValues(const char* value, int valueIndex) = 0;

		void SetParent(K::GameObject* newParent);
	};
}