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

	Audio::Audio(float frequency, float amplitude)
	{
		ma_waveform_config config = ma_waveform_config_init(ma_format_f32, 1, 48000, ma_waveform_type_sine, amplitude, frequency);
		ma_result result = ma_waveform_init(&config, &this->waveform);
		if (result != MA_SUCCESS) 
		{
			std::cout << "Cannot load sound: " << frequency << "hz" << std::endl;
		}
		ma_sound_init_from_data_source(&K::window->miniAudioEngine, &this->waveform, 0, NULL, &this->loadedAudio);
	}

	void Audio::Play(bool replay)
	{
		if(replay)
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
		ma_waveform_uninit(&this->waveform);
	}
}