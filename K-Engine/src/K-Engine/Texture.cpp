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
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
		stbi_image_free(this->image);
		glActiveTexture(0);
		glBindTexture(this->type, 0);
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
				glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
		glDeleteTextures(1, &this->id);
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

	void Texture::LoadAnimation() 
	{
		this->Bind(0);
		unsigned char* image01 = stbi_xload_file(this->GetFilePath(), &this->width, &this->height, &this->frames, &this->delay);
		if (this->frames > 1)
		{
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image01);
			std::cout << "Number Of Frames: " << this->frames << " Delay: " << this->delay << std::endl;
		}
		stbi_image_free(image01);
		this->Unbind();
	}
}