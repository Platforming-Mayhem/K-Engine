#pragma once
#include "build.h"
#include "RenderTexture.h"

namespace K 
{
	struct TextureInfo 
	{
		int id;
		int viewId;
		int type;
		int frames;
		int width;
		int height;
		int fps;
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
		unsigned char* image;
		bool loadedAnimation = false;
		bool loadedTexture = false;
		int width;
		int height;
		int frames;
		int fps;
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

		unsigned int GetID() 
		{
			return this->id;
		}

		unsigned int GetViewID() 
		{
			return this->viewId;
		}

		std::string GetFilePath();

		std::string GetTextureManagerName();

		unsigned char* GetFrameImage(int frameIndex);

		int GetFrameRate();

		void JoinThread();

		void LoadIntoGPU();

		void Bind(const GLint texture_unit);

		void Unbind();

		void LoadAnimation();

		void Load();
	};
}