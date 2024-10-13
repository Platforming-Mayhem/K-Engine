#pragma once
#include "../build.h"

namespace K 
{
	struct K_API SerialiseObject 
	{
	private:
		std::string data;

	public:

		void AppendInt(int value) 
		{
			std::string temp((char*)&value, sizeof(int));
			temp += '\0';
			data.append(temp);
		}

		void AppendFloat(float value) 
		{
			std::string temp((char*)&value, sizeof(float));
			temp += '\0';
			data.append(temp);
		}

		void AppendBool(bool value) 
		{
			int intValue = value;
			std::string temp((char*)&intValue, sizeof(int));
			temp += '\0';
			data.append(temp);
		}

		void AppendString(std::string value) 
		{
			std::string temp = value;
			temp += '\0';
			data.append(temp);
		}

		std::string ConvertToBinary() 
		{
			return data;
		}
	};

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

		virtual const char* GetName();

		virtual const char* GetPropertyValues() = 0;

		virtual void SetPropertyValues(const char* value, int valueIndex) = 0;

		void SetParent(K::GameObject* newParent);
	};
}