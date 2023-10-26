#include "Material.h"

namespace K 
{
	Material::Material()
	{
		this->shader = K::Shader(CORE_SHADER);
	}
	K::Shader* Material::GetShader() 
	{
		return &this->shader;
	}
}