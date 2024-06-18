#pragma once
#include "iostream"
#include<fmod.hpp>
#include<fmod.h>
#include<fmod_errors.h>

class AudioManager
{
public:

	AudioManager(FMOD_RESULT& result, FMOD::System*& system, FMOD::Sound*& sound);
	void PlaySound(const char* path, FMOD::System*& system, FMOD::Sound*& sound, FMOD_MODE mode);

};