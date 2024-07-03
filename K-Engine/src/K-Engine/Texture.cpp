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
		this->type = type;
		this->filename = filename + std::to_string(this->type);
		if (this->textures->Contains(this->filename))
		{
			K::TextureInfo tInfo = *this->textures->Check(this->filename);
			this->id = tInfo.id;
			this->viewId = tInfo.viewId;
			this->type = tInfo.type;
			this->frames = tInfo.frames;
			this->fps = tInfo.fps;
			this->width = tInfo.width;
			this->height = tInfo.height;
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else
		{
			stbi_set_flip_vertically_on_load(true);
			if (this->type == GL_TEXTURE_2D_ARRAY)
			{
				this->thread = std::thread(&Texture::LoadAnimation, this);
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
				this->thread = std::thread(&Texture::Load, this);
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
			textureInfo.dependencies++;
			textureInfo.dependenciesPointers.push_back(this);
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::Texture(unsigned int resource, GLenum type) {
		this->textures = &K::textureManager;
		this->type = type;
		this->filename = std::to_string(resource) + std::to_string(this->type);
		if (this->textures->Contains(this->filename))
		{
			K::TextureInfo tInfo = *this->textures->Check(this->filename);
			this->id = tInfo.id;
			this->viewId = tInfo.viewId;
			this->type = tInfo.type;
			this->frames = tInfo.frames;
			this->fps = tInfo.fps;
			this->width = tInfo.width;
			this->height = tInfo.height;
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else 
		{
			stbi_set_flip_vertically_on_load(true);
			if (this->type == GL_TEXTURE_2D_ARRAY) 
			{
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
						glGenTextures(1, &this->viewId);
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
			textureInfo.dependencies++;
			textureInfo.dependenciesPointers.push_back(this);
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::~Texture() 
	{
		//std::cout << "Begin Texture Destruction..." << this->filename << "," << this->textures->Check(this->filename)->dependencies << std::endl;
		if (this->textures->Check(this->filename)->dependencies > 0)
		{
			int count = 0;
			for (auto i : this->textures->Check(this->filename)->dependenciesPointers)
			{
				if (i == (void*)this)
				{
					this->textures->Check(this->filename)->dependenciesPointers.erase(this->textures->Check(this->filename)->dependenciesPointers.begin() + count);
					this->textures->Check(this->filename)->dependencies--;
					break;
				}
				else 
				{
					count++;
				}
			}
		}
		if (this->textures->Check(this->filename)->dependencies <= 0)
		{
			this->textures->Remove(this->filename);
			if (this->viewId != this->id)
			{
				glDeleteTextures(1, &this->viewId);
			}
			glDeleteTextures(1, &this->id);
		}
		//std::cout << "End Texture Destruction..." << std::endl;
	}

	std::string Texture::GetFilePath() 
	{
		int size = this->filename.size() - std::to_string(this->type).size();
		std::string newFilename = this->filename.substr(0, size);
		std::ifstream temp(ASSET_DIR + newFilename);
		if (temp.good())
			return newFilename;
		else
			return "";
	}

	std::string Texture::GetTextureManagerName() 
	{
		return this->filename;
	}

	void Texture::JoinThread() 
	{
		if (this->thread.joinable())
		{
			this->thread.join();
			//std::cout << this->filename << std::endl;
		}
	}

	void Texture::LoadIntoGPU() 
	{
		this->JoinThread();
		glBindTexture(this->type, this->id);
		if (this->loadedAnimation && this->image != nullptr)
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
				}
			}
			this->loadedAnimation = false;
		}
		if (this->loadedTexture && this->image != nullptr)
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
		glBindTexture(this->type, 0);
	}

	void Texture::Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		this->LoadIntoGPU();
		glBindTexture(this->type, this->id);
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
		std::string temp = ASSET_DIR + this->GetFilePath();
		this->image = stbi_xload_file(temp.c_str(), &this->width, &this->height, &this->frames, &this->delay);
		if(this->delay != nullptr)
			this->fps = 1.0f / (*this->delay / 1000.0f);
		if (this->image) 
		{
			this->loadedAnimation = true;
		}
		else 
		{
			std::cerr << "Failed to load animation: " << this->GetFilePath() << std::endl;
		}
	}

	void Texture::Load() 
	{
		std::string temp = ASSET_DIR + this->GetFilePath();
		this->image = stbi_load(temp.c_str(), &this->width, &this->height, &this->c, 0);
		if (this->image) 
		{
			this->loadedTexture = true;
		}
		else 
		{
			std::cerr << "Failed to load texture: " << this->GetFilePath() << std::endl;
		}
	}
}