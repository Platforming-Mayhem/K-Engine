#pragma once
#include "build.h"

namespace K 
{
	class K_API Image 
	{
	private:
		unsigned char* imageData;
		int delay;
	public:
		Image(unsigned char* data, int delay) 
		{
			this->imageData = data;
			this->delay = delay;
		}

		~Image() 
		{
			
		}

		unsigned char* GetData() 
		{
			return this->imageData;
		}
	};

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
		std::vector<Image> images;
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

		int GetNumberOfFrames() 
		{
			return this->frames;
		}

		const char* GetFilePath();

		unsigned char* GetFrameImage(int frameIndex);

		int GetFrameDelay(int frame);

		void Bind(const GLint texture_unit);

		void Unbind();

		void LoadFrame(int frame);

		void LoadAnimation();
	};
}