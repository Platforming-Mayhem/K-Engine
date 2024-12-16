#pragma once
#include "Shader.h"

namespace K 
{
	struct K_API MaterialInfo
	{
		int id;
		int dependencies = 0;
		std::vector<void*> dependenciesPointers;
	};

	class K_API MaterialManager
	{
	public:
		std::map<std::string, K::MaterialInfo> materials;

		MaterialManager();

		void Remove(std::string location);

		void Add(std::string location, K::MaterialInfo id);

		K::MaterialInfo* Check(std::string location);

		bool Contains(std::string location);

		~MaterialManager();
	};

	extern K_API K::MaterialManager materialManager;

	class K_API Material
	{
	private:
		K::MaterialManager* materials;
		std::string filename;
		K::Shader shader;
	public:
		K::Shader* GetShader();
		std::string GetLocation();
		Material(std::string location);
		~Material();
	};
}