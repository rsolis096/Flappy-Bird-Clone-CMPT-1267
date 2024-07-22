#pragma once

#include <random>
#include "Game.h"

struct Level
{
	Level(SDL_Renderer *);
	~Level();
	SDL_Texture *topObjectTexture;
	SDL_Surface *topObjectSurface;

	SDL_Texture *bottomObjectTexture;
	SDL_Surface *bottomObjectSurface;

	std::vector<SDL_Rect> topObjects;
	std::vector<SDL_Rect> bottomObjects;
	std::vector<SDL_Rect> pointObject;

	std::vector<int> diff{-150, -100, -50, 50, 100, 150, -100, -150, -200};
	int diffIndex;

	void drawLevel(SDL_Renderer *);
	void updatePosition(int);
	void restartGame();
	int getRandom(int, int);
};
