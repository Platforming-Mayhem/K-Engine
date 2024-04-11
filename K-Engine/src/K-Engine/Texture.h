#pragma once
#include "build.h"

namespace K 
{
	struct TextureInfo 
	{
		int id;
		int type;
		int frames;
	};

	class K_API TextureManager 
	{
	private:
		std::map<std::string, K::TextureInfo> textures;
	public:
		TextureManager();
		
		void Remove(std::string location);

		void Add(std::string location, K::TextureInfo id);

		K::TextureInfo Check(std::string location);

		bool Contains(std::string location);

		~TextureManager();
	};

	extern K::TextureManager textureManager;

	class K_API Texture
	{
	private:
		K::TextureManager* textures;
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
		bool isLooping = true;

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

		void LoadAnimation();
	};
}