#include "Collider.h"
#include "Time.h"

namespace K 
{
	Collider::Collider() 
	{
		
	}

	Collider::~Collider()
	{
		
	}

	void Collider::Init()
	{
		
	}

	void Collider::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Collider Settings")) 
		{
			ImGui::Checkbox("Is Static", &this->isStatic);
		}
	}

	void Collider::Update()
	{
		
	}

	void Collider::Unbind()
	{

	}

	void Collider::Bind()
	{
		
	}

	void Collider::SetPropertyValues(const char* value)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			std::string a(1, temp.back());
			int propertyNumber = std::stoi(a);
			temp.pop_back();
			switch (propertyNumber)
			{
			case 0:
				if (temp == "true")
				{
					this->SetStaticState(true);
				}
				else if (temp == "false")
				{
					this->SetStaticState(false);
				}
				break;
			}
		}
	}

	void Collider::SetStaticState(bool state) 
	{
		this->isStatic = state;
	}

	const char* Collider::GetPropertyValues() 
	{
		std::string temp;
		if (this->isStatic)
		{
			temp = "true";
		}
		else
		{
			temp = "false";
		}
		this->properties = temp;
		return this->properties.c_str();
	}

	const char* Collider::GetName()
	{
		return typeid(K::Collider).name();
	}
}