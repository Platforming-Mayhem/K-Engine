#pragma once
#include "build.h"

namespace K 
{
	class K_API Audio 
	{
	private:
		ma_sound loadedAudio;
	public:

		Audio(const char* path);

		void PlayLooping();

		void StopLooping();

		void Play();

		void Stop();

		~Audio();
	};
}