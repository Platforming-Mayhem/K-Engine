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
	public:
		Deserializer(K::Scene* newScene, std::string location);
	};
}