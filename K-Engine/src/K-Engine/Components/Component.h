#pragma once
#include "../build.h"

namespace K 
{
	struct K_API SerialiseObject 
	{
		std::vector<int> ints;
		std::vector<float> floats;
		std::vector<bool> bools;
		std::vector<std::string> strings;

		std::string ConvertToBinary() 
		{
			std::string data;
			for (auto i : ints) 
			{
				std::string temp((char*) &i, sizeof(int));
				temp += '\0';
				data.append(temp);
			}
			for (auto j : floats)
			{
				std::string temp((char*) &j, sizeof(float));
				temp += '\0';
				data.append(temp);
			}
			for (auto k : bools)
			{
				int value = k;
				std::string temp((char*)&value, sizeof(int));
				temp += '\0';
				data.append(temp);
			}
			for (auto l : strings)
			{
				std::string temp = l;
				temp += '\0';
				data.append(temp);
			}
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