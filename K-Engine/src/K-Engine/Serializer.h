#pragma once
#include "build.h"
#include "Scene.h"
#include "Editor.h"

namespace K 
{
	class K_API Serializer 
	{
	public:
		Serializer(K::Scene* scene);
	};

	class K_API Deserializer 
	{
	public:
		Deserializer(K::Scene* newScene, std::string location, K::Editor* editor);

		Deserializer(K::Scene* newScene, unsigned int resource, K::Editor* editor);
	};
}