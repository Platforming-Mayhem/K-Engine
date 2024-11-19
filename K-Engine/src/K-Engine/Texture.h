#pragma once
#include "build.h"
#include "RenderTexture.h"

namespace K 
{
	struct TextureInfo 
	{
		int dependencies = 0;
		std::vector<void*> dependenciesPointers;
	};

	class K_API TextureManager 
	{
	public:
		std::map<std::string, K::TextureInfo> textures;

		TextureManager();

		void Remove(std::string location);

		void Add(std::string location, K::TextureInfo id);

		K::TextureInfo* Check(std::string location);

		bool Contains(std::string location);

		~TextureManager();
	};

	extern K::TextureManager textureManager;

	class K_API Texture
	{
	private:
		std::thread thread;
		K::TextureManager* textures;
		unsigned int id;
		unsigned int viewId;
		unsigned int uploadPBO;
		unsigned char* image;
		bool loadedAnimation = false;
		bool loadedTexture = false;
		int width;
		int height;
		int frames;
		int fps;
		int* delay;
		int c;
		std::string filename;

	public:
		bool isLooping = true;

		Texture(const char* filename);

		Texture(unsigned int resource);

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

		unsigned int GetID() 
		{
			return this->id;
		}

		unsigned int GetViewID() 
		{
			return this->viewId;
		}

		std::string GetFilePath();

		unsigned char* GetFrameImage(int frameIndex);

		void updatePixels(GLubyte* dst, int size)
		{
			static int color = 0;

			if (!dst)
				return;

			int* ptr = (int*)dst;

			// copy 4 bytes at once
			for (int i = 0; i < this->height; ++i)
			{
				for (int j = 0; j < this->width; ++j)
				{
					*ptr = color;
					++ptr;
				}
				color += 257;
			}
			++color;            // scroll down
		}

		int GetFrameRate();

		void LoadIntoGPU();

		void Bind(const GLint texture_unit);

		void Unbind();

		void LoadAnimation();

		void Load();

		void LoadJIMAGE(std::string filename);

		void LoadJANIM(std::string filename);

		void CreateJIMAGE();

		void CreateJANIM();
	};
}