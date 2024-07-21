#pragma once
#include "Game.h"

struct Background
{
	Background();
	~Background();
	const char* fileName[2];
	SDL_Rect BG1;
	SDL_Rect BG2;
	SDL_Texture* bgImage[2];
	SDL_Surface* bgSurface[2];
	void drawBackground(SDL_Renderer*);
	void loadBackground(SDL_Renderer*, int);
	void updatePosition(int);
	void restartGame();
	int startingBG1;
	int startingBG2;
};

Background::Background()
{
	fileName[0] = "Assets/textures/bg1.png";
	fileName[1] = "Assets/textures/bg2.png";
	bgImage[0] = nullptr;
	bgSurface[0] = nullptr;
	bgImage[1] = nullptr;
	bgSurface[1] = nullptr;
	BG1 = { 0,0, 1024, 1024 };
	BG2 = { 1024,0,1024,1024 };
	startingBG1 = 0;
	startingBG2 = 1024;
}

void Background::loadBackground(SDL_Renderer* mRenderer, int n)
{
	SDL_DestroyTexture(bgImage[n]);
	bgSurface[n] = IMG_Load(fileName[n]);
	if (bgSurface[n] == nullptr )
		SDL_Log("Background failed to load: %s", SDL_GetError());
	else
	{
		bgImage[n] = SDL_CreateTextureFromSurface(mRenderer, bgSurface[n]);
		if (bgImage[n] == nullptr )
			SDL_Log("Background failed to load: %s", SDL_GetError());
	}
	SDL_FreeSurface(bgSurface[n]);
}

void Background::drawBackground(SDL_Renderer* mRenderer)
{
	//So the camera focuses on the player properly
	//Draw the character on screen
	if (BG1.x <= startingBG1 - 1024)
	{
		BG1.x = BG2.x + 1024;
		startingBG1 = BG1.x;	
	}
	if (BG2.x <= startingBG2 - 2048)
	{
		BG2.x = BG1.x + 1024;
		startingBG2 = BG2.x;
	}
	SDL_RenderCopy(mRenderer, bgImage[0], NULL, &BG1);
	SDL_RenderCopy(mRenderer, bgImage[1], NULL, &BG2);
	
}

void Background::updatePosition(int offset)
{
	BG1.x -= offset;
	BG2.x -= offset;
}

Background::~Background()
{
	for (int i = 0; i < 2; i++)
	{
		SDL_DestroyTexture(bgImage[i]);
		bgImage[i] = nullptr;
		bgSurface[i] = nullptr;
	}
}

void Background::restartGame()
{
	BG1 = { 0,0, 1024, 1024 };
	BG2 = { 1024,0,1024,1024 };
	startingBG1 = 0;
	startingBG2 = 1024;
}
