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

	Texture::Texture(const char* filename)
	{
		this->textures = &K::textureManager;
		this->filename = filename;
		if (this->textures->Contains(this->filename))
		{
			K::Texture* parentTex = ((K::Texture*)this->textures->Check(this->filename)->dependenciesPointers[0]);
			this->id = parentTex->GetID();
			this->viewId = parentTex->GetViewID();
			this->frames = parentTex->GetNumberOfFrames();
			this->fps = parentTex->GetFrameRate();
			this->width = parentTex->GetWidth();
			this->height = parentTex->GetHeight();
			this->c = parentTex->GetChannels();
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else
		{
			stbi_set_flip_vertically_on_load(true);

			glGenTextures(1, &this->id);
			glGenBuffers(1, &this->PBO);

			if (this->filename.contains(".gif"))
			{
				std::string temp = ASSET_DIR + this->GetFilePath();
				K::ImageSize imageMeta = this->ReadDimensionsAnimation(temp.c_str());

				this->width = imageMeta.width;
				this->height = imageMeta.height;
				this->frames = imageMeta.frames;
				this->fps = imageMeta.c;

				glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, this->width, this->height, this->frames);

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->PBO);
				glBufferData(GL_PIXEL_UNPACK_BUFFER, this->width * this->height * this->frames * 4 * sizeof(unsigned char), 0, GL_STREAM_DRAW);

				this->image = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

				this->thread = std::thread(&Texture::LoadAnimation, this);
				this->thread.detach();
			}
			else
			{
				std::string temp = ASSET_DIR + this->GetFilePath();
				K::ImageSize imageMeta = this->ReadDimensions(temp.c_str());

				this->width = imageMeta.width;
				this->height = imageMeta.height;
				this->c = imageMeta.c;

				glBindTexture(GL_TEXTURE_2D, this->id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				if (this->c > 3)
				{
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->width, this->height);
				}
				else
				{
					glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, this->width, this->height);
				}

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->PBO);
				glBufferData(GL_PIXEL_UNPACK_BUFFER, this->width * this->height * this->c * sizeof(unsigned char), 0, GL_STREAM_DRAW);

				this->image = (unsigned char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

				this->thread = std::thread(&Texture::Load, this);
				this->thread.detach();

				this->viewId = this->id;
			}
			K::TextureInfo textureInfo = K::TextureInfo();
			textureInfo.dependencies++;
			textureInfo.dependenciesPointers.push_back(this);
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::Texture(unsigned int resource) {
		this->textures = &K::textureManager;
		this->filename = std::to_string(resource);
		if (this->textures->Contains(this->filename))
		{
			K::TextureInfo tInfo = *this->textures->Check(this->filename);
			K::Texture* parentTex = ((K::Texture*)this->textures->Check(this->filename)->dependenciesPointers[0]);
			this->id = parentTex->GetID();
			this->viewId = parentTex->GetViewID();
			this->frames = parentTex->GetNumberOfFrames();
			this->fps = parentTex->GetFrameRate();
			this->width = parentTex->GetWidth();
			this->height = parentTex->GetHeight();
			this->c = parentTex->GetChannels();
			this->textures->Check(this->filename)->dependencies++;
			this->textures->Check(this->filename)->dependenciesPointers.push_back(this);
		}
		else 
		{
			stbi_set_flip_vertically_on_load(true);
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
					glBindTexture(GL_TEXTURE_2D, this->id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					if (this->image)
					{
						if (this->c > 3)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
						}
						else
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
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
			K::TextureInfo textureInfo = K::TextureInfo();
			textureInfo.dependencies++;
			textureInfo.dependenciesPointers.push_back(this);
			this->textures->Add(this->filename, textureInfo);
		}
	}

	Texture::~Texture() 
	{
		if(this->image != nullptr)
			delete this->image;
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
			if (this->PBO)
				glDeleteBuffers(1, &this->PBO);
			glDeleteTextures(1, &this->id);
		}
		//std::cout << "End Texture Destruction..." << std::endl;
	}

	std::string Texture::GetFilePath() 
	{
		return this->filename;
	}

	void Texture::LoadIntoGPU() 
	{
		if (this->loadedAnimation)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->PBO);

			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			this->image = nullptr;

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, this->width, this->height, this->frames, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			glDeleteBuffers(1, &this->PBO);

			glGenTextures(1, &this->viewId);
			glTextureView(this->viewId, GL_TEXTURE_2D, this->id, GL_RGBA8, 0, 1, 0, 1);

			if (this->textures->Check(this->filename)->dependencies - 1 > 0)
			{
				for (auto i : this->textures->Check(this->filename)->dependenciesPointers)
				{
					if (i != this) 
					{
						((K::Texture*)i)->viewId = this->viewId;
						((K::Texture*)i)->fps = this->fps;
						((K::Texture*)i)->frames = this->frames;
						((K::Texture*)i)->width = this->width;
						((K::Texture*)i)->height = this->height;
					}
				}
			}
			this->loadedAnimation = false;
		}
		if (this->loadedTexture)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->PBO);

			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			this->image = nullptr;

			if (this->c > 3)
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			}
			else
			{
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, 0);
			}

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			glDeleteBuffers(1, &this->PBO);

			if (this->textures->Check(this->filename)->dependencies - 1 > 0)
			{
				for (auto i : this->textures->Check(this->filename)->dependenciesPointers)
				{
					if (i != this) 
					{
						((K::Texture*)i)->width = this->width;
						((K::Texture*)i)->height = this->height;
						((K::Texture*)i)->c = this->c;
					}
				}
			}
			this->loadedTexture = false;
		}
	}

	void Texture::Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		if (this->filename.contains(".gif"))
		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);
		}
		else 
		{
			glBindTexture(GL_TEXTURE_2D, this->id);
		}
		this->LoadIntoGPU();
	}

	void Texture::Unbind() 
	{
		if (this->filename.contains(".gif"))
		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	unsigned char* Texture::GetFrameImage(int frame) 
	{
		return this->image + (this->width * this->height * 4 * frame);
	}

	int Texture::GetFrameRate()
	{
		return this->fps;
	}

	void Texture::LoadJIMAGE(std::string filename)
	{
		int found = filename.find('.');
		if (found != std::string::npos)
		{
			std::string temp1 = std::filesystem::path(filename).replace_extension(".JIMG").string();
			std::ifstream file(temp1, std::ios::binary | std::ios::in);
			std::string imageTemp;
			if (file.is_open())
			{
				std::string info;
				std::getline(file, info);
				int sizeOfInfo = info.length() + 1;
				file.close();

				this->width = std::stoi(info.substr(0, info.find(".")));
				info.erase(0, info.find(".") + 1);
				this->height = std::stoi(info.substr(0, info.find(".")));
				info.erase(0, info.find(".") + 1);
				this->c = std::stoi(info);

				//this->image = (unsigned char*)std::calloc(this->width * this->height * this->c, sizeof(unsigned char));
				if (this->image != nullptr) 
				{
					std::FILE* fastFile = std::fopen(temp1.c_str(), "rb");
					std::fseek(fastFile, sizeOfInfo, SEEK_SET);
					int bytesRead = std::fread(this->image, sizeof(unsigned char), this->width * this->height * this->c, fastFile);
					std::fclose(fastFile);
				}
			}
		}
	}

	void Texture::LoadJANIM(std::string filename)
	{
		std::string temp1 = std::filesystem::path(filename).replace_extension(".JANIM").string();
		std::ifstream file(temp1, std::ios::binary | std::ios::in);
		if (file.is_open())
		{

			std::string info;
			std::getline(file, info);
			int sizeOfInfo = info.length() + 1;
			file.close();

			this->width = std::stoi(info.substr(0, info.find(".")));
			info.erase(0, info.find(".") + 1);
			this->height = std::stoi(info.substr(0, info.find(".")));
			info.erase(0, info.find(".") + 1);
			this->frames = std::stoi(info.substr(0, info.find(".")));
			info.erase(0, info.find(".") + 1);
			this->fps = std::stoi(info);

			//this->image = (unsigned char*)std::calloc(this->width * this->height * 4 * this->frames, sizeof(unsigned char));
			if (this->image != nullptr)
			{
				std::FILE* fastFile = std::fopen(temp1.c_str(), "rb");
				std::fseek(fastFile, sizeOfInfo, SEEK_SET);
				int bytesRead = std::fread(this->image, sizeof(unsigned char), this->width * this->height * 4 * this->frames, fastFile);
				std::fclose(fastFile);
			}
		}
	}

	void Texture::LoadAnimation() 
	{
		std::string temp = ASSET_DIR + this->GetFilePath();

		unsigned char* originalData = this->image;

		#if _DEBUG
			this->image = stbi_xload_file(temp.c_str(), &this->width, &this->height, &this->frames, &this->delay);
			this->fps = 1.0f / (*this->delay / 1000.0f);
			this->CreateJANIM();
			stbi_image_free(this->image);
		#endif

		this->image = originalData;
		this->LoadJANIM(temp);

		if (this->image) 
		{
			this->loadedAnimation = true;
		}
		else 
		{
			std::cerr << "Failed to load animation: " << temp << std::endl;
		}
	}

	void Texture::Load() 
	{
		std::string temp = ASSET_DIR + this->GetFilePath();

		unsigned char* originalData = this->image;

		#if _DEBUG
			this->image = stbi_load(temp.c_str(), &this->width, &this->height, &this->c, 0);
			this->CreateJANIM();
			stbi_image_free(this->image);
		#endif

		this->image = originalData;
		this->LoadJIMAGE(temp);
		
		if (this->image) 
		{
			this->loadedTexture = true;
		}
		else 
		{
			std::cerr << "Failed to load texture: " << temp << std::endl;
		}
	}

	void Texture::CreateJIMAGE() 
	{
		std::string location = this->GetFilePath();
		std::filesystem::path tempPath = ASSET_DIR + location;
		int timeSinceEpoch = std::chrono::duration_cast<std::chrono::minutes>(std::filesystem::last_write_time(tempPath).time_since_epoch()).count();
		int found = location.find('.');
		if (found != std::string::npos)
		{
			location = ASSET_DIR + location.substr(0, found) + ".JIMG";
			tempPath = location;
			if ((std::filesystem::exists(tempPath) && std::chrono::duration_cast<std::chrono::minutes>(std::filesystem::last_write_time(tempPath).time_since_epoch()).count() < timeSinceEpoch)|| !std::filesystem::exists(tempPath))
			{
				std::cout << "Updating/Creating at:" << location << std::endl;
				std::ofstream outFile;
				outFile.open(location, std::ios::binary | std::ios::out);
				if (!outFile)
				{
					std::cerr << "Error - unable to open output file " << location.c_str() << std::endl;
					exit(1);
				}
				outFile << this->width << "." << this->height << "." << this->c << "\n";
				outFile.close();
				std::FILE* fastFile = std::fopen(location.c_str(), "ab");
				std::fwrite(this->image, sizeof(unsigned char), this->width * this->height * this->c, fastFile);
				std::fclose(fastFile);
			}
		}
	}

	void Texture::CreateJANIM()
	{
		std::string location = this->GetFilePath();
		std::filesystem::path tempPath = ASSET_DIR + location;

		int timeSinceEpoch = std::chrono::duration_cast<std::chrono::minutes>(std::filesystem::last_write_time(tempPath).time_since_epoch()).count();
		int found = location.find('.');
		if (found != std::string::npos)
		{
			location = ASSET_DIR + location.substr(0, found) + ".JANIM";
			tempPath = location;
			if ((std::filesystem::exists(tempPath) && std::chrono::duration_cast<std::chrono::minutes>(std::filesystem::last_write_time(tempPath).time_since_epoch()).count() < timeSinceEpoch) || !std::filesystem::exists(tempPath))
			{
				std::cout << "Updating/Creating at:" << location << std::endl;
				std::ofstream outFile;
				outFile.open(location, std::ios::binary | std::ios::out);
				if (!outFile)
				{
					std::cerr << "Error - unable to open output file " << location.c_str() << std::endl;
					exit(1);
				}
				outFile << this->width << "." << this->height << "." << this->frames << "." << this->fps << "\n";
				outFile.close();
				std::FILE* fastFile = std::fopen(location.c_str(), "ab");
				std::fwrite(this->image, sizeof(unsigned char), this->width * this->height * 4 * this->frames, fastFile);
				std::fclose(fastFile);
			}
		}
	}
}