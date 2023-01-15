#pragma once

#include <random>
#include "Game.h"

struct Level
{
	Level(SDL_Renderer*);
	~Level();
	SDL_Texture* topObjectTexture;
	SDL_Surface* topObjectSurface;

	SDL_Texture* bottomObjectTexture;
	SDL_Surface* bottomObjectSurface;

	vector<SDL_Rect> topObjects;
	vector<SDL_Rect> bottomObjects;
	vector<SDL_Rect> pointObject;

	vector<int> diff{ -150, -100, -50,50, 100, 150, -100, -150, -200};
	int diffIndex;

	void drawLevel(SDL_Renderer*);
	void updatePosition(int);
	void restartGame();
	int getRandom(int, int);

};

int Level::getRandom(int start, int end)
{
	std::random_device myEngine;
	std::uniform_real_distribution<double> randomVal(start, end);
		int toReturn = static_cast<int>(randomVal(myEngine));
		//cout << toReturn << endl;
	return toReturn;
}


Level::Level(SDL_Renderer* mRenderer) : diffIndex(0)
{
	//Set of objects		{x,y,w,h}
	topObjects.push_back({ 600,-500,150,900 }); //Top
	bottomObjects.push_back({ 600,700,150,900 }); //Bottom

	topObjects.push_back({ 1200,-500,150,900 });//Top
	bottomObjects.push_back({ 1200,650,150,900 });//Bottom

	pointObject.push_back({ 0, 0, 5, 10000 });
	pointObject.push_back({ 100, 0, 5, 10000 });

	topObjectSurface = IMG_Load("Assets/textures/pipeTop.png");
	topObjectTexture = SDL_CreateTextureFromSurface(mRenderer, topObjectSurface);
	SDL_FreeSurface(topObjectSurface);

	bottomObjectSurface = IMG_Load("Assets/textures/pipeBottom.png");
	bottomObjectTexture = SDL_CreateTextureFromSurface(mRenderer, bottomObjectSurface);
	SDL_FreeSurface(bottomObjectSurface);

}

void Level::drawLevel(SDL_Renderer* mRenderer)
{
	diffIndex = getRandom(0, 8);
	//Render pipes with opening facing downwards, moves them based on random diff
	for (auto& element: topObjects)
	{
		if (element.x > -150)
			SDL_RenderCopy(mRenderer, topObjectTexture, NULL, &element);
		else
		{
			element.x += 1200;
			element.y = -500;
			element.y += diff[diffIndex];
		}

	}

	//Render the pipes with opening facing upwards, moves them based on random diff
	for (auto& element: bottomObjects)
	{
		if (element.x > -150)
			SDL_RenderCopy(mRenderer, bottomObjectTexture, NULL, &element);
		else
		{
			element.x += 1200;
			element.y = 650;
			element.y += diff[diffIndex];
		}
	}
	//Move point markers, not rendered
	pointObject[0].x = topObjects[0].x + topObjects[0].w/2;
	pointObject[1].x = topObjects[1].x + topObjects[0].w/2;		
}

void Level::updatePosition(int offset)
{
	for (SDL_Rect& object : topObjects)
		object.x -= offset;
	for (SDL_Rect& object : bottomObjects)
		object.x -= offset;

}

Level::~Level()
{
	SDL_DestroyTexture(topObjectTexture);
	SDL_DestroyTexture(bottomObjectTexture);
	//SDL_FreeSurface(topObjectSurface); 
	//SDL_FreeSurface(bottomObjectSurface);

	topObjectTexture = nullptr;
	topObjectSurface = nullptr;
	bottomObjectTexture = nullptr;
	bottomObjectSurface = nullptr;
}

void Level::restartGame()
{
	//Resets positions back to start
	topObjects[0] = {600,-500,150,900}; 
	topObjects[1] = {1200,-500,150,900};

	bottomObjects[0] = { 600,700,150,900 }; 
	bottomObjects[1] = { 1200,650,150,900 };

	pointObject[0] = { 0, 0, 5, 10000 };
	pointObject[1] = { 100, 0, 5, 10000 };
}