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
			K::Material* mat = ((K::Material*)this->materials->Check(this->filename)->dependenciesPointers[0]);
			this->shader = mat->shader;
			this->uniforms = mat->uniforms;
			this->materials->Check(this->filename)->dependencies++;
			this->materials->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else 
		{
			this->shader = new K::Shader(this->filename);
			this->uniforms = new std::unordered_map<std::string, int>();

			GLint i;
			GLint count;

			GLint size; // size of the variable
			GLenum type; // type of the variable (float, vec3 or mat4, etc)

			GLsizei bufSize = 16; // maximum name length
			glGetProgramiv(this->shader->shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

			std::vector<GLchar> name(bufSize); // variable name in GLSL
			GLsizei length; // name length

			glGetProgramiv(this->shader->shader, GL_ACTIVE_UNIFORMS, &count);
			printf("Active Uniforms: %d\n", count);

			for (i = 0; i < count; i++)
			{
				glGetActiveUniform(this->shader->shader, (GLuint)i, bufSize, &length, &size, &type, &name[0]);

				int id = glGetUniformLocation(this->shader->shader, &name[0]);

				this->AddUniform(&name[0], id);

				printf("Uniform #%d Type: %u Name: %s ID: %d\n", i, type, &name[0], id);
			}

			K::MaterialInfo info = K::MaterialInfo();
			info.dependencies++;
			info.dependenciesPointers.push_back(this);
			this->materials->Add(this->filename, info);
		}
	}

	Material::Material(int resource)
	{
		this->materials = &K::materialManager;
		this->filename = resource;
		if (this->materials->Contains(this->filename))
		{
			K::Material* mat = ((K::Material*)this->materials->Check(this->filename)->dependenciesPointers[0]);
			this->shader = mat->shader;
			this->uniforms = mat->uniforms;
			this->materials->Check(this->filename)->dependencies++;
			this->materials->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else
		{
			this->shader = new K::Shader(resource);
			this->uniforms = new std::unordered_map<std::string, int>();

			GLint i;
			GLint count;

			GLint size; // size of the variable
			GLenum type; // type of the variable (float, vec3 or mat4, etc)

			GLsizei bufSize = 16; // maximum name length
			glGetProgramiv(this->shader->shader, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);

			std::vector<GLchar> name(bufSize); // variable name in GLSL
			GLsizei length; // name length

			glGetProgramiv(this->shader->shader, GL_ACTIVE_UNIFORMS, &count);
			printf("Active Uniforms: %d\n", count);

			for (i = 0; i < count; i++)
			{
				glGetActiveUniform(this->shader->shader, (GLuint)i, bufSize, &length, &size, &type, &name[0]);
				
				int id = glGetUniformLocation(this->shader->shader, &name[0]);

				this->AddUniform(&name[0], id);

				printf("Uniform #%d Type: %u Name: %s ID: %d\n", i, type, &name[0], id);
			}

			K::MaterialInfo info = K::MaterialInfo();
			info.dependencies++;
			info.dependenciesPointers.push_back(this);
			this->materials->Add(this->filename, info);
		}
	}

	std::string Material::GetLocation() 
	{
		return this->filename;
	}

	void Material::AddUniform(std::string name, int uniform)
	{
		this->uniforms->insert(std::make_pair(name, uniform));
	}

	int Material::GetUniform(std::string name)
	{
		if (this->uniforms->contains(name)) 
		{
			return this->uniforms->at(name);
		}
		else 
		{
			return -1;
		}
	}

	void Material::RemoveUniform(std::string name) 
	{
		this->uniforms->erase(name);
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
			this->uniforms->clear();
			delete this->shader;
			delete this->uniforms;
			this->materials->Remove(this->filename);
		}
	}

	K::Shader* Material::GetShader() 
	{
		return this->shader;
	}
}