#include "K-Engine/Material.h"

namespace K 
{
	K::MaterialManager K::materialManager = K::MaterialManager();

	MaterialManager::MaterialManager() 
	{

	}

	void MaterialManager::Remove(std::string location)
	{
		this->materials.erase(location);
	}

	void MaterialManager::Add(std::string location, K::MaterialInfo id)
	{
		this->materials.insert(std::make_pair(location, id));
	}

	K::MaterialInfo* MaterialManager::Check(std::string location)
	{
		return &this->materials.find(location)->second;
	}

	bool MaterialManager::Contains(std::string location)
	{
		return this->materials.contains(location);
	}

	MaterialManager::~MaterialManager()
	{

	}

	Material::Material(std::string location)
	{
		this->materials = &K::materialManager;
		this->filename = location;
		if (this->materials->Contains(this->filename))
		{
			K::MaterialInfo tInfo = *this->materials->Check(this->filename);
			this->shader = K::Shader();
			this->shader.shader = tInfo.id;
			this->materials->Check(this->filename)->dependencies++;
			this->materials->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else 
		{
			this->shader = K::Shader(this->filename);
			K::MaterialInfo info = K::MaterialInfo();
			info.id = this->shader.shader;
			info.dependencies++;
			info.dependenciesPointers.push_back(this);
			this->materials->Add(this->filename, info);
		}
	}

	std::string Material::GetLocation() 
	{
		return this->filename;
	}

	Material::~Material() 
	{
		if (this->materials->Check(this->filename)->dependencies > 0)
		{
			int count = 0;
			for (auto i : this->materials->Check(this->filename)->dependenciesPointers)
			{
				if (i == (void*)this)
				{
					this->materials->Check(this->filename)->dependenciesPointers.erase(this->materials->Check(this->filename)->dependenciesPointers.begin() + count);
					this->materials->Check(this->filename)->dependencies--;
					break;
				}
				else
				{
					count++;
				}
			}
		}
		if (this->materials->Check(this->filename)->dependencies <= 0)
		{
			this->materials->Remove(this->filename);
		}
	}

	K::Shader* Material::GetShader() 
	{
		return &this->shader;
	}
}