#pragma once
#include "build.h"

namespace K 
{
	class K_API Shader
	{
	public:
		std::string shaderCode[2];
		unsigned int shader;

		Shader();

		Shader(int resource);

		Shader(std::string file);

		void AddUniform(std::string name, int uniform);

		GLint GetUniform(const char* name) const;

		int GetNumberOfUniforms();

		void RemoveUniform(std::string name);

		~Shader();

	private:

		std::unordered_map<std::string, int> uniformLocations;

		unsigned int CompileShader(unsigned int type, const std::string& source);

		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}