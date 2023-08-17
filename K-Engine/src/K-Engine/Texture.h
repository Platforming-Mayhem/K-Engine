#pragma once
#include "build.h"

namespace K 
{
	class K_API Texture
	{
	private:
		GLuint id;
		int width;
		int height;
		int c;
		unsigned int type;

	public:
		Texture(const char* filename, GLenum type);

		Texture(unsigned int resource, GLenum type);

		virtual ~Texture();

		int GetWidth() 
		{
			return this->width;
		}

		int GetHeight()
		{
			return this->height;
		}

		void Bind(const GLint texture_unit);

		void Unbind();
	};
}