#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace K 
{
	K::TextureManager K::textureManager = K::TextureManager();

	TextureManager::TextureManager() 
	{

	}

	void TextureManager::Add(std::string location, K::TextureInfo id)
	{
		this->textures.insert(std::make_pair(location, id));
	}

	K::TextureInfo TextureManager::Check(std::string location)
	{
		return this->textures.find(location)->second;
	}

	bool TextureManager::Contains(std::string location)
	{
		return this->textures.contains(location);
	}

	TextureManager::~TextureManager() 
	{

	}

	Texture::Texture(const char* filename, GLenum type)
	{
		this->textures = &K::textureManager;
		this->filename = filename;
		if (this->textures->Contains(this->filename)) 
		{
			this->id = this->textures->Check(this->filename).id;
			this->type = this->textures->Check(this->filename).type;
			this->frames = this->textures->Check(this->filename).frames;
		}
		else
		{
			this->type = type;
			stbi_set_flip_vertically_on_load(true);
			if (this->type == GL_TEXTURE_2D_ARRAY)
			{
				this->LoadAnimation();
				glGenTextures(1, &this->id);
				glBindTexture(this->type, this->id);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				if (this->image)
				{
					glTexImage3D(this->type, 0, GL_RGBA, this->width, this->height, this->frames, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
				}
				else
				{
					std::cout << "Failed to load texture" << std::endl;
				}
				glActiveTexture(0);
				glBindTexture(this->type, 0);
				stbi_image_free(this->image);
				this->image = nullptr;
				std::cout << "Loaded Animation: " << this->GetFilePath() << std::endl;
			}
			else
			{
				this->image = stbi_load((ASSET_DIR + this->filename).c_str(), &this->width, &this->height, &this->c, 0);
				glGenTextures(1, &this->id);
				glBindTexture(this->type, this->id);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				if (this->image)
				{
					if (this->c >= 4)
					{
						glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
					}
					else
					{
						glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
					}
					glActiveTexture(0);
					glBindTexture(this->type, 0);
					stbi_image_free(this->image);
					this->image = nullptr;
					std::cout << "Loaded Single Texture: " << this->GetFilePath() << std::endl;
				}
				else
				{
					std::cout << "Failed to load texture at: " << this->filename << std::endl;
				}
			}
			K::TextureInfo textureInfo = K::TextureInfo();
			textureInfo.id = this->id;
			textureInfo.type = this->type;
			textureInfo.frames = this->frames;
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::Texture(unsigned int resource, GLenum type) {
		stbi_set_flip_vertically_on_load(true);
		this->type = type;
		if (IS_INTRESOURCE(resource))
		{
			HMODULE hModule;
			GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "main", &hModule);
			HRSRC hr = FindResource(hModule, MAKEINTRESOURCE(resource), "PNG");
			int size = SizeofResource(hModule, hr);
			if (hr == NULL)
			{
				std::cout << "Failed to find resource" << std::endl;
				std::cout << size << std::endl;
			}
			else
			{
				HGLOBAL temp = LoadResource(hModule, hr);
				LPVOID lp = LockResource(temp);
				this->image = stbi_load_from_memory(static_cast<const stbi_uc*>(lp), size, &this->width, &this->height, &this->c, 0);
				glGenTextures(1, &this->id);
				glBindTexture(this->type, this->id);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				if (this->image)
				{
					if (this->c > 3)
					{
						glTexImage3D(this->type, 0, GL_RGBA, this->width, this->height, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
					}
					else
					{
						glTexImage3D(this->type, 0, GL_RGB, this->width, this->height, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
					}
					glActiveTexture(0);
					glBindTexture(this->type, 0);
					stbi_image_free(this->image);
					this->image = nullptr;
				}
				else
				{
					std::cout << "Failed to load texture" << std::endl;
				}
				UnlockResource(temp);
			}
		}
		else
		{
			std::cout << "Failed to find texture" << std::endl;
		}
	}

	Texture::~Texture() 
	{
		std::cout << "Begin Texture Destruction..." << std::endl;
		glDeleteTextures(1, &this->id);
		std::cout << "End Texture Destruction..." << std::endl;
	}

	const char* Texture::GetFilePath() 
	{
		return this->filename.c_str();
	}

	void Texture::Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
	}

	void Texture::Unbind() 
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	unsigned char* Texture::GetFrameImage(int frame) 
	{
		return this->image + (this->width * this->height * 4 * frame);
	}

	int Texture::GetFrameDelay(int frame) 
	{
		return this->delay[frame] / 10;
	}

	void Texture::LoadAnimation() 
	{
		this->image = stbi_xload_file((ASSET_DIR + this->filename).c_str(), &this->width, &this->height, &this->frames, &this->delay);
	}
}