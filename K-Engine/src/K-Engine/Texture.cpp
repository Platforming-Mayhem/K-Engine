#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace K 
{
	K::TextureManager K::textureManager = K::TextureManager();

	TextureManager::TextureManager() 
	{

	}

	void TextureManager::Remove(std::string location)
	{
		this->textures.erase(location);
	}

	void TextureManager::Add(std::string location, K::TextureInfo id)
	{
		this->textures.insert(std::make_pair(location, id));
	}

	K::TextureInfo* TextureManager::Check(std::string location)
	{
		return &this->textures.find(location)->second;
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
			this->id = this->textures->Check(this->filename)->id;
			this->viewId = this->textures->Check(this->filename)->viewId;
			this->type = this->textures->Check(this->filename)->type;
			this->frames = this->textures->Check(this->filename)->frames;
			this->fps = this->textures->Check(this->filename)->fps;
			this->width = this->textures->Check(this->filename)->width;
			this->height = this->textures->Check(this->filename)->height;
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else
		{
			this->type = type;
			stbi_set_flip_vertically_on_load(true);
			if (this->type == GL_TEXTURE_2D_ARRAY)
			{
				std::thread thread(&Texture::LoadAnimation, this);
				thread.detach();
				glGenTextures(1, &this->id);
				glBindTexture(this->type, this->id);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(this->type, 0);
			}
			else
			{
				std::thread thread(&Texture::Load, this);
				thread.detach();
				glGenTextures(1, &this->id);
				glBindTexture(this->type, this->id);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glBindTexture(this->type, 0);

				this->viewId = this->id;
			}
			K::TextureInfo textureInfo = K::TextureInfo();
			textureInfo.id = this->id;
			textureInfo.viewId = this->viewId;
			textureInfo.type = this->type;
			textureInfo.frames = this->frames;
			textureInfo.fps = this->GetFrameRate();
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::Texture(unsigned int resource, GLenum type) {
		this->textures = &K::textureManager;
		this->filename = std::to_string(resource);
		if (this->textures->Contains(this->filename))
		{
			this->id = this->textures->Check(this->filename)->id;
			this->viewId = this->textures->Check(this->filename)->viewId;
			this->type = this->textures->Check(this->filename)->type;
			this->frames = this->textures->Check(this->filename)->frames;
			this->fps = this->textures->Check(this->filename)->fps;
			this->width = this->textures->Check(this->filename)->width;
			this->height = this->textures->Check(this->filename)->height;
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else 
		{
			this->type = type;
			stbi_set_flip_vertically_on_load(true);
			if (this->type == GL_TEXTURE_2D_ARRAY) 
			{
				std::cout << "Set Texture 2D Array" << std::endl;
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
						this->frames = 1;
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
								glTexStorage3D(this->type, 1, GL_RGBA8, this->width, this->height, this->frames);
								glTexSubImage3D(this->type, 0, 0, 0, 0, this->width, this->height, this->frames, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
							}
							else
							{
								glTexStorage3D(this->type, 1, GL_RGB8, this->width, this->height, this->frames);
								glTexSubImage3D(this->type, 0, 0, 0, 0, this->width, this->height, this->frames, GL_RGB, GL_UNSIGNED_BYTE, this->image);
							}
							stbi_image_free(this->image);
							this->image = nullptr;

							glBindTexture(this->type, 0);

							glGenTextures(1, &this->viewId);

							if (this->c > 3) 
							{
								glTextureView(this->viewId, GL_TEXTURE_2D, this->id, GL_RGBA8, 0, 1, 0, 1);
							}
							else 
							{
								glTextureView(this->viewId, GL_TEXTURE_2D, this->id, GL_RGB8, 0, 1, 0, 1);
							}
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
			else 
			{
				std::cout << "Set Texture 2D" << std::endl;
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
								glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
							}
							else
							{
								glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
							}
							stbi_image_free(this->image);
							this->image = nullptr;
						}
						else
						{
							std::cout << "Failed to load texture" << std::endl;
						}
						this->viewId = this->id;
						UnlockResource(temp);
					}
				}
				else
				{
					std::cout << "Failed to find texture" << std::endl;
				}
			}
			K::TextureInfo textureInfo = K::TextureInfo();
			textureInfo.id = this->id;
			textureInfo.viewId = this->viewId;
			textureInfo.type = this->type;
			textureInfo.frames = this->frames;
			textureInfo.width = this->width;
			textureInfo.height = this->height;
			textureInfo.fps = this->GetFrameRate();
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::~Texture() 
	{
		//std::cout << "Begin Texture Destruction..." << std::endl;
		if (this->filename.empty()) 
		{
			glDeleteTextures(1, &this->id);
		}
		else 
		{
			if (this->textures->Check(this->filename)->dependencies <= 0)
			{
				if (this->textures->Contains(this->filename))
				{
					this->textures->Remove(this->filename);
				}
				glDeleteTextures(1, &this->id);
			}
			else
			{
				this->textures->Check(this->filename)->dependencies--;
				int count = 0;
				for (auto i : this->textures->Check(this->filename)->dependenciesPointers) 
				{
					if (i == this) 
					{
						this->textures->Check(this->filename)->dependenciesPointers.erase(this->textures->Check(this->filename)->dependenciesPointers.begin() + count);
					}
					count++;
				}
			}
		}
		if (this->viewId != this->id && this->viewId != NULL) 
		{
			glDeleteTextures(1, &this->viewId);
		}
		//std::cout << "End Texture Destruction..." << std::endl;
	}

	const char* Texture::GetFilePath() 
	{
		return this->filename.c_str();
	}

	void Texture::Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
		if (this->loadedAnimation)
		{
			glTexStorage3D(this->type, 1, GL_RGBA8, this->width, this->height, this->frames);
			glTexSubImage3D(this->type, 0, 0, 0, 0, this->width, this->height, this->frames, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
			stbi_image_free(this->image);
			this->image = nullptr;

			glGenTextures(1, &this->viewId);
			glTextureView(this->viewId, GL_TEXTURE_2D, this->id, GL_RGBA8, 0, 1, 0, 1);

			if (this->textures->Check(this->filename)->dependencies > 0) 
			{
				for (auto i : this->textures->Check(this->filename)->dependenciesPointers) 
				{
					((K::Texture*)i)->fps = this->fps;
					((K::Texture*)i)->frames = this->frames;
					((K::Texture*)i)->width = this->width;
					((K::Texture*)i)->height = this->height;
					((K::Texture*)i)->viewId = this->viewId;
				}
			}
			this->loadedAnimation = false;
		}
		if (this->loadedTexture) 
		{
			if (this->c >= 4)
			{
				glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
			}
			else
			{
				glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
			}
			stbi_image_free(this->image);
			this->image = nullptr;
			if (this->textures->Check(this->filename)->dependencies > 0)
			{
				for (auto i : this->textures->Check(this->filename)->dependenciesPointers)
				{
					((K::Texture*)i)->width = this->width;
					((K::Texture*)i)->height = this->height;
				}
			}
			this->loadedTexture = false;
		}
	}

	void Texture::Unbind() 
	{
		glBindTexture(this->type, 0);
	}

	unsigned char* Texture::GetFrameImage(int frame) 
	{
		return this->image + (this->width * this->height * 4 * frame);
	}

	int Texture::GetFrameRate()
	{
		return this->fps;
	}

	void Texture::LoadAnimation() 
	{
		this->image = stbi_xload_file((ASSET_DIR + this->filename).c_str(), &this->width, &this->height, &this->frames, &this->delay);
		if(this->delay != nullptr)
			this->fps = 1.0f / (*this->delay / 1000.0f);
		if (this->image) 
		{
			this->loadedAnimation = true;
		}
		else 
		{
			std::cerr << "Failed to load animation: " << this->filename << std::endl;
		}
	}

	void Texture::Load() 
	{
		this->image = stbi_load((ASSET_DIR + this->filename).c_str(), &this->width, &this->height, &this->c, 0);
		if (this->image) 
		{
			this->loadedTexture = true;
		}
		else 
		{
			std::cerr << "Failed to load texture: " << this->filename << std::endl;
		}
	}
}