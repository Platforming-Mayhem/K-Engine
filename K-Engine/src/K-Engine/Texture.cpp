#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace K 
{
	Texture::Texture(const char* filename, GLenum type)
	{
		stbi_set_flip_vertically_on_load(true);
		this->type = type;
		unsigned char* image = stbi_load(filename, &this->width, &this->height, &this->c, 0);
		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (image)
		{
			if (this->c >= 4) 
			{
				glTexImage2D(type, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			}
			else 
			{
				glTexImage2D(type, 0, GL_RGB8, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			}
			glGenerateTextureMipmap(type);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(image);
		glActiveTexture(0);
		glBindTexture(type, 0);
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
				unsigned char* image = stbi_load_from_memory(static_cast<const stbi_uc*>(lp), size, &this->width, &this->height, &this->c, 0);
				glGenTextures(1, &this->id);
				glBindTexture(type, this->id);
				glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				if (image)
				{
					if (this->c > 3)
					{
						glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
					}
					else
					{
						glTexImage2D(type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
					}
					glGenerateTextureMipmap(type);
				}
				else
				{
					std::cout << "Failed to load texture" << std::endl;
				}
				glActiveTexture(0);
				glBindTexture(type, 0);
				stbi_image_free(image);
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

	void Texture::Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
		glEnable(GL_TEXTURE_2D);
	}

	void Texture::Unbind() 
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
		glDisable(GL_TEXTURE_2D);
	}
}