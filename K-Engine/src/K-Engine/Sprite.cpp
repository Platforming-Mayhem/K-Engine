#include "Sprite.h"

namespace K 
{
	Sprite::Sprite()
	{
		
	}

	Sprite::~Sprite() 
	{
		//std::cout << "Begin Sprite Destruction..." << std::endl;
		delete this->texture;
		delete this->normalTexture;
		//std::cout << "End Sprite Destruction..." << std::endl;
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
		this->properties += "," + (std::string)this->normalTexture->GetFilePath();
		return this->properties.c_str();
	}

	void Sprite::ResetFrame() 
	{
		this->frame = 0;
		this->internalClock = 0.0f;
	}

	void Sprite::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
			{
				this->SetColorTexture(temp.c_str());
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
				if (this->canChromaKey) 
				{
					this->chromaKeyColour[0] = std::stof(temp);
				}
				else 
				{
					this->SetNormalTexture(temp.c_str());
					this->hasNormal = true;
				}
			}
			break;
			case 3:
			{
				if (this->canChromaKey)
				{
					this->chromaKeyColour[1] = std::stof(temp);
				}
				else
				{

				}
			}
			break;
			case 4:
			{
				if (this->canChromaKey)
				{
					this->chromaKeyColour[2] = std::stof(temp);
				}
				else
				{

				}
			}
			break;
			case 5:
			{
				if (this->canChromaKey)
				{
					this->SetNormalTexture(temp.c_str());
					this->hasNormal = true;
				}
				else 
				{

				}
			}
			break;
			}
		}
	}

	K::Texture* Sprite::GetTexture() 
	{
		return this->texture;
	}

	void Sprite::SetColorTexture(const char* value)
	{
		this->SetTexture(new K::Texture(value, GL_TEXTURE_2D_ARRAY), false);
	}

	void Sprite::SetNormalTexture(const char* value)
	{
		this->normalTexture = new K::Texture(value, GL_TEXTURE_2D);
	}

	void Sprite::SetColorTexture(unsigned int resource)
	{
		this->SetTexture(new K::Texture(resource, GL_TEXTURE_2D_ARRAY), false);
	}

	void Sprite::SetNormalTexture(unsigned int resource)
	{
		this->normalTexture = new K::Texture(resource, GL_TEXTURE_2D);
	}

	void Sprite::SetTexture(K::Texture* newTexture, bool reScale) 
	{
		if (this->texture != newTexture) 
		{
			this->frame = 0;
			this->internalClock = 0.0f;
			this->texture = newTexture;
			if (reScale) 
			{
				this->parent->GetTransform()->scale->x = this->texture->GetWidth() / 32.0f;
				this->parent->GetTransform()->scale->z = this->texture->GetHeight() / 32.0f;
			}
		}
	}

	bool Sprite::IsPlaying() 
	{
		return this->isPlaying;
	}

	void Sprite::RenderInit() 
	{
		if (this->texture == nullptr)
		{
			this->SetColorTexture(WATERMARK);
		}
		if (this->normalTexture == nullptr)
		{
			this->SetNormalTexture(WATERMARK);
		}
	}

	void Sprite::Init()
	{
		
	}

	void Sprite::RenderBind() 
	{
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), this->canChromaKey);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasNormal"), this->hasNormal);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "chromaKey"), this->chromaKeyColour[0], this->chromaKeyColour[1], this->chromaKeyColour[2]);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), true);

		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "texture0"), 0);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "texture1"), 1);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "frame"), this->frame);

		this->texture->Bind(0);

		this->normalTexture->Bind(1);
	}

	void Sprite::Render() 
	{
		
	}

	void Sprite::RenderUnbind() 
	{
		this->texture->Unbind();
		this->normalTexture->Unbind();
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "frame"), 0);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasNormal"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
	}

	void Sprite::Bind() 
	{
		
	}

	void Sprite::Unbind()
	{
		
	}

	int Sprite::GetFrameNumber() 
	{
		return this->frame;
	}

	void Sprite::NextFrame() 
	{
		if (this->texture->GetNumberOfFrames() > 1)
		{
			if (this->texture->isLooping) 
			{
				if (this->frame < this->texture->GetNumberOfFrames() - 1)
				{
					this->frame++;
				}
				else
				{
					this->frame = 0;
				}
				this->isPlaying = true;
			}
			else 
			{
				if (this->frame < this->texture->GetNumberOfFrames() - 1)
				{
					this->frame++;
					this->isPlaying = true;
				}
				else 
				{
					this->frame = this->texture->GetNumberOfFrames() - 1;
					this->isPlaying = false;
				}
			}
		}
	}

	void Sprite::Update()
	{
		float fps = 1.0f / this->texture->GetFrameRate();
		if (this->internalClock >= fps)
		{
			this->NextFrame();
			this->internalClock = 0.0f;
		}
		this->internalClock += K::Time::deltaTime();
	}

	void Sprite::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Sprite Settings"))
		{
			/*ImGui::Text("These are the sprite settings");
			if (ImGui::Button("Load New Sprite"))
			{
				file.SetTitle("Load Sprite");
				file.SetTypeFilters({ ".PNG", ".JPG", ".GIF"});
				file.SetPwd(ASSET_DIR);
				file.Open();
			}
			if (ImGui::Button("Load Normal Sprite")) 
			{
				fileNormal.SetTitle("Load Normal Sprite");
				fileNormal.SetTypeFilters({ ".PNG", ".JPG", ".GIF" });
				fileNormal.SetPwd(ASSET_DIR);
				fileNormal.Open();
			}
			ImGui::Text("FPS: %i", this->texture->GetFrameRate());
			ImGui::Checkbox("has Normal Texture", &this->hasNormal);
			ImGui::Checkbox("Can Chroma Key", &this->canChromaKey);
			if (this->canChromaKey) 
			{
				ImGui::ColorPicker3("Chroma Key Colour", this->chromaKeyColour);
			}
			file.Display();
			fileNormal.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
				this->SetColorTexture(relativeLocation.c_str());
				file.ClearSelected();
			}
			if (fileNormal.HasSelected()) 
			{
				std::string location = fileNormal.GetSelected().string();
				std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
				this->SetNormalTexture(relativeLocation.c_str());
				this->hasNormal = true;
				fileNormal.ClearSelected();
			}*/
			ImGui::Text("%p,%i,%i",this->texture->GetViewID(), this->texture->GetWidth(), this->texture->GetHeight());
			ImGui::Image((void*)(intptr_t)(this->texture->GetViewID()), ImVec2(128.0f, 128.0f), ImVec2(0,1), ImVec2(1,0), ImVec4(1,1,1,1), ImVec4(0,0,0,1));
			ImGui::Text("%p,%i,%i", this->normalTexture->GetViewID(), this->normalTexture->GetWidth(), this->normalTexture->GetHeight());
			ImGui::Image((void*)(intptr_t)(this->normalTexture->GetViewID()), ImVec2(128.0f, 128.0f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 1));
		}
	}
}