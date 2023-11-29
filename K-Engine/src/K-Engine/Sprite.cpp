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
		this->properties = this->texture->GetFilePath();
		if (this->canChromaKey) 
		{
			this->properties += ",true";
			this->properties += "," + std::to_string(this->chromaKeyColour[0]);
			this->properties += "," + std::to_string(this->chromaKeyColour[1]);
			this->properties += "," + std::to_string(this->chromaKeyColour[2]);
		}
		else 
		{
			this->properties += ",false";
		}
		return this->properties.c_str();
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
			{
				K::Texture* tempTex = new K::Texture(temp.c_str(), GL_TEXTURE_2D);
				this->SetTexture(tempTex);
			}
			break;
			case 1:
			{
				if (temp == "true")
				{
					this->canChromaKey = true;
				}
				else if (temp == "false")
				{
					this->canChromaKey = false;
				}
			}
			break;
			case 2:
			{
				this->chromaKeyColour[0] = std::stof(temp);
			}
			break;
			case 3:
			{
				this->chromaKeyColour[1] = std::stof(temp);
			}
			break;
			case 4:
			{
				this->chromaKeyColour[2] = std::stof(temp);
			}
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
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), this->canChromaKey);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "chromaKey"), this->chromaKeyColour[0], this->chromaKeyColour[1], this->chromaKeyColour[2]);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), true);
		this->texture->Bind(0);
	}

	void Sprite::Unbind()
	{
		this->texture->Unbind();
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
		int time = this->internalClock * 60.0f;
		int fps = (1.0f / 24.0f) * 60.0f;
		if (time % fps == 0)
		{
			this->NextFrame();
		}
		this->internalClock += K::Time::deltaTime();
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
			ImGui::Checkbox("Can Chroma Key", &this->canChromaKey);
			if (this->canChromaKey) 
			{
				ImGui::ColorPicker3("Chroma Key Colour", this->chromaKeyColour);
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