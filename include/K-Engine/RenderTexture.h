#pragma once
#include "build.h"
namespace K 
{
	class K_API RenderTexture
	{
	private:
		unsigned int colourID;
		unsigned int depthID;
		unsigned int frameBufferID;
		unsigned int type;
		int width;
		int height;
	public:
		RenderTexture(int width, int height, GLenum type, GLenum internalFormat = GL_RGB, GLenum format = GL_RGB);

		~RenderTexture();

		void Bind();

		void Unbind();

		unsigned int GetFramebufferID() 
		{
			return this->frameBufferID;
		}

		unsigned int GetID() 
		{
			return this->colourID;
		}

		int GetWidth() 
		{
			return this->width;
		}

		int GetHeight() 
		{
			return this->height;
		}
	};
}