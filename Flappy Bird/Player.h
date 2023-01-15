#pragma once

#include "Game.h"
#include <vector>
#include <string>

class Player
{
	public:
		Player(const char*, int, int);
		~Player();//override
		void getInput(const Uint8*);
		void UpdateActor(float);
		void drawActor(SDL_Renderer*, int);
		void loadTexture(SDL_Renderer*);
		void flap();
		void restartGame();
		SDL_Rect getHitBox();
		int getPositionX();
		int getPositionY();
		int getWidth();
		int getHeight();

		bool isAlive;
		double rotation;
		bool allowRotation;

protected:
		//Position Attributes
		Vector2 actorPosition;
		SDL_Rect hitBox;


		//Attributes
		float actorHeight;
		float actorWidth;
		const char* fileName;
		bool isJumping;
		bool isFalling;


		//Movement Related
		Vector2 delta;
		float jumpPower;
		int movementMultiplier;		
		float hoverSpeed;

		//Texture Related
		SDL_Texture* sprite[2];
		SDL_Surface* spriteSurface[2];
};
