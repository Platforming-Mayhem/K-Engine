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
			std::string temp = value;
			std::string a(1, temp.back());
			int propertyNumber = std::stoi(a);
			temp.pop_back();
			switch (propertyNumber)
			{
			case 0:
				K::Texture * tempTex = new K::Texture(temp.c_str(), GL_TEXTURE_2D);
				this->SetTexture(tempTex);
				break;
			}
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
		this->internalClock += K::Time::deltaTime();
	}

	void Sprite::NextFrame() 
	{
		if (this->texture->GetNumberOfFrames() > 1)
		{
			if (this->frame < this->texture->GetNumberOfFrames() - 1)
			{
				this->frame++;
			}
			else
			{
				this->frame = 0;
			}
			this->texture->LoadFrame(this->frame);
		}
	}

	void Sprite::Update()
	{
		int time = this->internalClock * 60;
		int fps = (1.0f / 24.0f) * 60;
		if (time % fps == 0)
		{
			this->NextFrame();
		}
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