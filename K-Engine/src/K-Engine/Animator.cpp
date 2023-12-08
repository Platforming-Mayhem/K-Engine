#include "Animator.h"

namespace K 
{
	K::Animator::Animator() 
	{

	}

	K::Animator::~Animator() 
	{
		std::cout << "Animator Destructor" << std::endl;
		this->animations.clear();
		this->animations.shrink_to_fit();
	}

	void K::Animator::Init() 
	{

	}

	void K::Animator::Bind() 
	{

	}

	void K::Animator::PlayAnimation(int index, K::Sprite* currentSprite) 
	{
		if (index < this->animations.size() && currentSprite->GetTexture() != this->animations[index]) 
		{
			currentSprite->SetTexture(this->animations[index]);
		}
	}

	void K::Animator::Update() 
	{
		
	}

	void K::Animator::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Animator Settings")) 
		{
			if (ImGui::Button("Add Animation")) 
			{
				file.SetTitle("Load Sprite");
				file.SetTypeFilters({ ".PNG", ".JPG", ".GIF" });
				file.Open();
			}
			file.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				K::Texture* temp = new K::Texture(location.c_str(), GL_TEXTURE_2D);
				this->animations.push_back(temp);
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
			}
		}
	}

	void K::Animator::Unbind() 
	{

	}

	void K::Animator::SetPropertyValues(const char* value, int valueIndex) 
	{

	}

	const char* K::Animator::GetName() 
	{
		return typeid(K::Animator).name();
	}

	const char* K::Animator::GetPropertyValues() 
	{
		return this->properties.c_str();
	}
}