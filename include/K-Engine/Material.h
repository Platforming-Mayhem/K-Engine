#pragma once
#include "Shader.h"

namespace K 
{
	struct K_API MaterialInfo
	{
		int dependencies = 0;
		std::vector<void*> dependenciesPointers;
	};

	class K_API MaterialManager
	{
	public:
		std::unordered_map<std::string, K::MaterialInfo> materials;

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
		std::unordered_map<std::string, GLint>* uniforms;
		K::Shader* shader;
	public:
		K::Shader* GetShader();
		std::string GetLocation();
		void AddUniform(std::string name, GLint uniform);
		GLint GetUniform(const std::string& name) const;
		int GetNumberOfUniforms();
		void RemoveUniform(std::string name);
		Material(std::string location);
		Material(int resource);
		~Material();
	};
}