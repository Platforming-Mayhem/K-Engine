#pragma once
#include "build.h"
#include "Scene.h"

namespace K 
{
	class K_API Serializer 
	{
	public:
		Serializer(K::Scene* scene, std::string location);
	};

	class K_API Deserializer 
	{
	private:
		K::Component* selectedComponent = nullptr;
		K::GameObject* selectedGameObject = nullptr;

		int componentDataCount = 0;
		std::map<K::GameObject*, int> parents;

	public:
		Deserializer(K::Scene* newScene, std::string location);

		void CreateComponent(K::GameObject* tempGameObject, std::vector<std::string>& data);

		void CreateComponentFast(std::string datum);

		void CreateGameObject(std::string gameObject);

		void CreateGameObjectFast(std::vector<std::string>& data);
	};
}