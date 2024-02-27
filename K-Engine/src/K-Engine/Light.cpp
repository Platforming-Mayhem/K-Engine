#include "Light.h"
#include "GameObject.h"

namespace K 
{
	Light::Light() 
	{

	}

	Light::~Light() 
	{

	}

	void Light::Init() 
	{

	}

	void Light::Update()
	{

	}

	void Light::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Light Settings")) 
		{
			ImGui::DragFloat3("Light Direction", &this->lightDirection.x);
		}
	}

	void Light::Bind()
	{
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "lightDirection"), this->lightDirection.x, this->lightDirection.y, this->lightDirection.z);
	}

	void Light::Unbind()
	{
	
	}
	void Light::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				this->lightDirection.x = std::stof(temp);
				break;
			case 1:
				this->lightDirection.y = std::stof(temp);
				break;
			case 2:
				this->lightDirection.z = std::stof(temp);
				break;
			}
		}
	}
	const char* Light::GetPropertyValues()
	{
		this->properties = std::to_string(this->lightDirection.x);
		this->properties += "," + std::to_string(this->lightDirection.y);
		this->properties += "," + std::to_string(this->lightDirection.z);
		return this->properties.c_str();
	}
	const char* Light::GetName()
	{
		return typeid(K::Light).name();
	}
}