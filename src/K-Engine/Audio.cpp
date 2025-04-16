#include "K-Engine/Audio.h"
#include "K-Engine/Window.h"

namespace K 
{
	Audio::Audio(const char* path, bool isLooping)
	{
		std::cout << "Loading sound: " << path << std::endl;
		this->isLooping = isLooping;
		ma_result result = ma_sound_init_from_file(&K::window->miniAudioEngine, path, 0, NULL, NULL, &this->loadedAudio);
		if (result != MA_SUCCESS)
		{
			std::cout << "Cannot load sound: " << path << std::endl;
		}
		ma_sound_set_looping(&this->loadedAudio, this->isLooping);
	}

	void Audio::Play()
	{
		ma_sound_seek_to_pcm_frame(&this->loadedAudio, 0);
		ma_sound_start(&this->loadedAudio);
	}

	void Audio::Stop()
	{
		ma_sound_stop(&this->loadedAudio);
	}

	Audio::~Audio() 
	{
		ma_sound_uninit(&this->loadedAudio);
	}
}