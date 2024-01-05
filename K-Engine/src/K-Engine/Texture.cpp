#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace K 
{
	Texture::Texture(const char* filename, GLenum type)
	{
		this->filename = filename;
		this->type = type;
		stbi_set_flip_vertically_on_load(true);
		this->image = stbi_load(filename, &this->width, &this->height, &this->c, 0);
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
			glGenerateTextureMipmap(this->type);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		glActiveTexture(0);
		glBindTexture(this->type, 0);
		stbi_image_free(this->image);
		this->image = nullptr;
		this->LoadAnimation();
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
						glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->image);
					}
					else
					{
						glTexImage2D(type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
					}
					glGenerateTextureMipmap(type);
				}
				else
				{
					std::cout << "Failed to load texture" << std::endl;
				}
				glActiveTexture(0);
				glBindTexture(this->type, 0);
				stbi_image_free(this->image);
				this->image = nullptr;
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
		stbi_image_free(this->image);
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

	void Texture::LoadFrame(int frame) 
	{
		this->Bind(0);

		glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->GetFrameImage(frame));

		this->Unbind();
	}

	void Texture::LoadAnimation() 
	{
		this->Bind(0);
		this->image = stbi_xload_file(this->GetFilePath(), &this->width, &this->height, &this->frames, &this->delay);
		if (this->frames <= 1)
		{
			stbi_image_free(this->image);
			this->image = nullptr;
		}
		this->Unbind();
	}
}