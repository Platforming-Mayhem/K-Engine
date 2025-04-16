#pragma once
#include "build.h"

namespace K 
{
	class K_API Audio 
	{
	private:
		ma_sound loadedAudio;
		bool isLooping = false;
	public:

		Audio(const char* path, bool isLooping = false);

		void Play();

		void Stop();

		~Audio();
	};
}