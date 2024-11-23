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

	struct ImageSize 
	{
		int width, height, c;

		ImageSize() 
		{
			this->width = -1;
			this->height = -1;
			this->c = -1;
		}

		ImageSize(int newWidth, int newHeight, int newC) 
		{
			this->width = newWidth;
			this->height = newHeight;
			this->c = newC;
		}
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
		unsigned int PBO;
		unsigned char* image;
		bool loadedAnimation = false;
		bool loadedTexture = false;
		int width;
		int height;
		int frames;
		int c;
		int fps;
		int* delay;
		std::string filename;

	public:
		bool isLooping = true;

		Texture(const char* filename);

		Texture(unsigned int resource);

		~Texture();

		int GetChannels() 
		{
			return this->c;
		}

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

		ImageSize ReadDimensions(const char* filename) 
		{
			#if _DEBUG
				std::ifstream file;
				file.open(filename, std::ios::binary);
				if (!file)
					return ImageSize();
				file.seekg(16, std::ios::beg);
				unsigned char data[10];
				file.read(reinterpret_cast<char*>(&data), 10);
				file.close();

				//Convert Character sequence to integer
				int valX = int((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
				int valY = int((data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7]);
				if (int(data[9]) == 2)
				{
					return ImageSize(valX, valY, 3);
				}
				else if (int(data[9]) == 6)
				{
					return ImageSize(valX, valY, 4);
				}
			#else
				std::string temp = std::filesystem::path(filename).replace_extension(".JIMG").string();
				std::ifstream file;
				file.open(temp.c_str(), std::ios::binary);
				if (!file)
					return ImageSize();
				std::string info;
				std::getline(file, info);
				file.close();

				int w = std::stoi(info.substr(0, info.find(".")));
				info.erase(0, info.find(".") + 1);
				int h = std::stoi(info.substr(0, info.find(".")));
				info.erase(0, info.find(".") + 1);
				int c = std::stoi(info);

				return ImageSize(w, h, c);
			#endif
		}

		int ReadWidth(const char* filename) 
		{
			std::ifstream file;
			file.open(filename, std::ios::binary);
			if (!file)
				return -1;
			file.seekg(16, std::ios::beg);
			unsigned char data[4];
			file.read(reinterpret_cast<char*>(&data), 4);
			file.close();

			//Convert Character sequence to integer
			int val = int((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);

			return val;
		}

		int ReadHeight(const char* filename)
		{
			std::ifstream file;
			file.open(filename, std::ios::binary);
			if (!file)
				return -1;
			file.seekg(20, std::ios::beg);
			unsigned char data[4];
			file.read(reinterpret_cast<char*>(&data), 4);
			file.close();

			//Convert Character sequence to integer
			int val = int((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);

			return val;
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