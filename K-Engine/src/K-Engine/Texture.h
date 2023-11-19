#pragma once
#include "build.h"

namespace K 
{
	class K_API Texture
	{
	private:
		unsigned int id;
		unsigned char* image;
		int width;
		int height;
		int frames;
		int* delay;
		int c;
		unsigned int type;
		std::string filename;

	public:
		Texture(const char* filename, GLenum type);

		Texture(unsigned int resource, GLenum type);

		~Texture();

		int GetWidth() 
		{
			return this->width;
		}

		int GetHeight()
		{
			return this->height;
		}

		const char* GetFilePath();

		void Bind(const GLint texture_unit);

		void Unbind();

		void LoadAnimation();
	};
}