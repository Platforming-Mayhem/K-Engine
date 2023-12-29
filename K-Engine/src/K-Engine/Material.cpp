#include "Material.h"

namespace K 
{
	Material::Material()
	{
		this->shader = K::Shader(CORE_SHADER);
	}

	Material::~Material() 
	{

	}

	K::Shader* Material::GetShader() 
	{
		return &this->shader;
	}
}