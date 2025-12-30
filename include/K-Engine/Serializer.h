#pragma once
#include "build.h"
#include "Scene.h"

namespace K 
{
	class K_API Serializer 
	{
	public:
		Serializer(K::Scene* scene, std::string location);
		std::vector<std::string> Split(std::string s, std::string delimiter);
		void RunTimeSerialize(K::Scene* scene, std::string location);
	};

	class K_API Deserializer 
	{
	private:
		K::Component* selectedComponent = nullptr;
		K::GameObject* selectedGameObject = nullptr;

		int componentDataCount = 0;

	public:
		std::unordered_map<K::GameObject*, int> parents;

		Deserializer(K::Scene* newScene, std::string location);

		void RunTimeDeserializer(K::Scene* newScene, std::string location);

		void EditorDeserializer(K::Scene* newScene, std::string location);

		void CreateComponent(K::GameObject* tempGameObject, std::vector<std::string>& data);

		void CreateComponentFast(std::string datum);

		void CreateGameObject(std::string gameObject);

		void CreateGameObjectFast(std::vector<std::string>& data);

		std::string LoadPrefab(std::string fileLocation);

		
	};

	struct K_API SerialiseObject
	{
	private:
		std::vector<int> ints;
		std::vector<float> floats;
		std::vector<bool> bools;
		std::vector<std::string> strings;
	public:

		SerialiseObject();

		SerialiseObject(K::GameObject* other);

		void CreateGameObject(Deserializer* deserializer);

		void AppendInt(int value)
		{
			this->ints.push_back(value);
		}

		void RemoveInt(int index) 
		{
			this->ints.erase(this->ints.begin() + index);
		}

		int GetInt(int index)
		{
			return this->ints.at(index);
		}

		int GetNumberOfInts()
		{
			return this->ints.size();
		}

		void AppendFloat(float value)
		{
			this->floats.push_back(value);
		}

		float GetFloat(int index)
		{
			return this->floats.at(index);
		}

		int GetNumberOfFloats()
		{
			return this->floats.size();
		}

		void AppendVector3(K::Vector3 value)
		{
			this->floats.push_back(value.x);
			this->floats.push_back(value.y);
			this->floats.push_back(value.z);
		}

		void AppendBool(bool value)
		{
			this->bools.push_back(value);
		}

		bool GetBool(int index)
		{
			return this->bools.at(index);
		}

		int GetNumberOfBools()
		{
			return this->bools.size();
		}

		void AppendString(std::string value)
		{
			this->strings.push_back(value);
		}

		std::string GetString(int index)
		{
			return this->strings.at(index);
		}

		int GetNumberOfStrings()
		{
			return this->strings.size();
		}
	};

	std::istream& operator>>(std::istream& is, K::SerialiseObject& sO);

	std::ostream& operator<<(std::ostream& os, K::SerialiseObject& sO);
}