#pragma once
#include "build.h"
#include "Shader.h"

namespace K 
{
	class K_API Material
	{
	private:
		K::Shader shader;
	public:
		K::Shader* GetShader();
		Material();
		~Material();
	};
}