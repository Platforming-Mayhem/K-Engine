#pragma once
#include "build.h"

namespace K 
{
	class K_API Audio 
	{
	private:
		ma_sound loadedAudio;
		ma_waveform waveform;
		bool isLooping = false;
	public:

		Audio(const char* path, bool isLooping = false);

		Audio(float frequency, float amplitude);

		void Play(bool replay = true);

		void Stop();

		~Audio();
	};
}