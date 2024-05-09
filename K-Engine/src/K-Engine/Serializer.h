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
		std::map<K::GameObject*, int> parents;

	public:
		Deserializer(K::Scene* newScene, std::string location);

		void CreateGameObject(std::string gameObject);
	};
}