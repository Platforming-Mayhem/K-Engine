#include "Animator.h"

namespace K 
{
	K::Animator::Animator() 
	{

	}

	K::Animator::~Animator() 
	{
		//std::cout << "Begin Animator Destructor..." << std::endl;
		if (!this->animations.empty())
		{
			for (int i = 0; i < this->animations.size(); i++) 
			{
				if(this->animations[i] != this->currentTexture)
					delete this->animations[i];
			}
			this->animations.clear();
			this->animations.shrink_to_fit();
			this->currentTexture = nullptr;
		}
		//std::cout << "End Animator Destructor..." << std::endl;
	}

	void K::Animator::Init() 
	{
		
	}

	void K::Animator::Bind() 
	{
		
	}

	void K::Animator::PlayAnimation(int index, K::Sprite* currentSprite, bool reScale) 
	{
		if (index < this->animations.size()) 
		{
			currentSprite->SetTexture(this->animations[index], reScale);
			this->currentTexture = this->animations[index];
		}
	}

	void Animator::AssignTexture(const char* value)
	{
		this->animations.push_back(new K::Texture(value, GL_TEXTURE_2D_ARRAY));
	}

	void K::Animator::Update() 
	{
		
	}

	void K::Animator::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Animator Settings")) 
		{
			/*if (ImGui::Button("Add Animation"))
			{
				file.SetTitle("Load Sprite");
				file.SetTypeFilters({ ".PNG", ".JPG", ".GIF" });
				file.SetPwd(ASSET_DIR);
				file.Open();
			}
			file.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				std::string relativeLocation = std::filesystem::relative(location, ASSET_DIR).string();
				this->AssignTexture(relativeLocation.c_str());
				this->selectedTexture = this->animations.size() - 1;
				file.ClearSelected();
			}
			if (ImGui::BeginListBox("Animations"))
			{
				for (int i = 0; i < this->animations.size(); i++)
				{
					if (ImGui::Selectable(this->animations[i]->GetFilePath()))
					{
						this->selectedTexture = i;
					}
				}
				ImGui::EndListBox();
			}
			if (this->selectedTexture < this->animations.size()) 
			{
				ImGui::Text(this->animations[this->selectedTexture]->GetFilePath());
				ImGui::Checkbox("Is Looping", &this->animations[this->selectedTexture]->isLooping);
			}
			if (ImGui::Button("Delete Animation")) 
			{
				this->animations.erase(this->animations.begin() + this->selectedTexture);
			}*/
		}
	}

	void K::Animator::Unbind() 
	{

	}

	void K::Animator::SetPropertyValues(const char* value, int valueIndex) 
	{
		if (value[0] != '\0' && value != nullptr) 
		{
			std::string temp = value;
			if (temp == "true")
			{
				this->animations.back()->isLooping = true;
			}
			else if (temp == "false")
			{
				this->animations.back()->isLooping = false;
			}
			else 
			{
				this->AssignTexture(value);
			}
		}
	}

	const char* K::Animator::GetName() 
	{
		return typeid(K::Animator).name();
	}

	const char* K::Animator::GetPropertyValues() 
	{
		this->properties = "";
		if (!this->animations.empty()) 
		{
			for (K::Texture* temp : this->animations)
			{
				this->properties += temp->GetFilePath();
				this->properties += ",";
				if (temp->isLooping)
				{
					this->properties += "true,";
				}
				else
				{
					this->properties += "false,";
				}
			}
			this->properties.pop_back();
		}
		return this->properties.c_str();
	}
}