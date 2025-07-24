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

		Shader(std::string file);

		GLint GetUniform(const char* name) const;

		~Shader();

	private:

		unsigned int CompileShader(unsigned int type, const std::string& source);

		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
