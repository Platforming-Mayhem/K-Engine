#include "RenderTexture.h"
namespace K 
{
	RenderTexture::RenderTexture(int width, int height, GLenum type)
	{
		this->width = width;
		this->height = height;
		this->type = type;

		glGenFramebuffers(1, &this->frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferID);

		glGenTextures(1, &this->colourID);
		glBindTexture(this->type, this->colourID);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colourID, 0);

		glBindTexture(this->type, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTexture::Bind() 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBufferID);
	}

	void RenderTexture::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	RenderTexture::~RenderTexture()
	{
		glDeleteFramebuffers(1, &this->frameBufferID);
		glDeleteTextures(1, &this->colourID);
	}
}