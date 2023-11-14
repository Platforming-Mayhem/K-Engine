#include "Sprite.h"

namespace K 
{
	Sprite::Sprite(K::Texture* newTexture)
	{
		this->SetTexture(newTexture);
	}

	Sprite::~Sprite() 
	{
		this->texture->~Texture();
	}

	const char* Sprite::GetName() 
	{
		return typeid(K::Sprite).name();
	}

	const char* Sprite::GetPropertyValues()
	{
		return this->texture->GetFilePath();
	}

	void Sprite::SetPropertyValues(const char* value)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			K::Texture* temp = new K::Texture(value, GL_TEXTURE_2D);
			this->SetTexture(temp);
		}
	}

	void Sprite::SetTexture(K::Texture* newTexture) 
	{
		this->texture = newTexture;
	}

	void Sprite::Init() 
	{

	}

	void Sprite::Bind() 
	{
		this->texture->Bind(0);
	}

	void Sprite::Unbind()
	{
		this->texture->Unbind();
	}

	void Sprite::Update()
	{

	}

	void Sprite::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Sprite Settings"))
		{
			ImGui::Text("These are the sprite settings");
			if (ImGui::Button("Load New Sprite"))
			{
				file.SetTitle("Load Sprite");
				file.SetTypeFilters({ ".PNG", ".JPG", ".GIF"});
				file.Open();
			}
			file.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				K::Texture* temp = new K::Texture(location.c_str(), GL_TEXTURE_2D);
				this->SetTexture(temp);
				file.ClearSelected();
			}
		}
	}
}