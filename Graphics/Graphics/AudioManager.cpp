#include "AudioManager.h"

AudioManager::AudioManager(FMOD_RESULT& result, FMOD::System*& system, FMOD::Sound*& sound)
{
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)
	{
		std::cout << "FMOD is not ok\n";
		exit(-1);
	}

	result = system->init(32, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void AudioManager::PlaySound(const char* path, FMOD::System*& system, FMOD::Sound*& sound, FMOD_MODE mode)
{
	system->createSound(path, mode, NULL, &sound);
	system->playSound(sound, NULL, false, NULL);
}
