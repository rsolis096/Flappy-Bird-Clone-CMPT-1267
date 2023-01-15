#pragma once
#include "Game.h"

struct SoundEffect
{
	SoundEffect(const char*, int, int, int);
	~SoundEffect();
	Mix_Chunk* sE;
	int loops;
	int channel;
	int volume;
	void playSound();
	void stopSound();
	void resumeSound();
};

SoundEffect::SoundEffect(const char* fN, int c, int l, int v)
{
	loops = l;
	channel = c;
	volume = v;
	sE = Mix_LoadWAV(fN);
	if (sE == nullptr)
		SDL_Log("Sound Effect failed to load: %s", SDL_GetError());
}

void SoundEffect::playSound()
{
	Mix_VolumeChunk(sE, volume);
	Mix_PlayChannel(channel, sE, loops);
}

void SoundEffect::stopSound()
{
	Mix_Pause(channel);
}

void SoundEffect::resumeSound()
{
	Mix_Resume(channel);
}

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(sE);
	sE = nullptr;

}