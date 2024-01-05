#include "Player.h"

const float gravity = 0.45;

Player::Player(const char* fN, int x, int y)
{


	//Position Related
	actorPosition.x = 100.0f;
	actorPosition.y = 500;
	actorHeight = static_cast<int>(x);
	actorWidth = static_cast<int>(y);
	hitBox = { static_cast<int>(actorPosition.x), static_cast<int>(actorPosition.y), static_cast<int>(actorWidth), static_cast<int>(actorHeight) };
	rotation = 0.0;

	//Movement Related
	jumpPower = 0;
	hoverSpeed = 0.1;
	movementMultiplier = 0;
	delta = { 0.0f, 0.0f };

	//Texture Related
	fileName = fN;
	sprite[1] = nullptr;
	sprite[0] = nullptr;
	spriteSurface[1] = nullptr;
	spriteSurface[0] = nullptr;

	//Bool
	isAlive = true;
	isJumping = false;
	isFalling = false;
	allowRotation = true;
}

void Player::getInput(const Uint8* state)
{
	if (state[SDL_SCANCODE_SPACE] && isAlive == true)
	{
		if (delta.y > -7)
		{
			isJumping = true;
			delta.y += -1.5f; //This delta represents the amount of jumping ability the player has.
		}
	}		
}

//update positon of player
void Player::UpdateActor(float deltaTime)
{

	if (isJumping == true)
	{
		//The player moves upwards, simulating a jump
		delta.y += gravity;//Gravity is pushing the jump downwards
		actorPosition.y += delta.y;
		//When the player reaches the peak of the jump, begin fall.
		if (delta.y == 0)
		{
			isJumping == false;
			isFalling = true;
		}
	}

	if (isFalling == true)
	{
		//Rate of falling
		jumpPower = 0;
		delta.y += gravity/3; //Make the rhs larger, player falls faster
		actorPosition.y += delta.y;
	}

	//Idle position, used at start of game
	if (isFalling == false && isJumping == false)
	{
		jumpPower = 0;
		delta.y = 0;
	}
	//Prevent player from falling through the floor
	if (static_cast<int>(actorPosition.y + actorHeight / 2) >= 956)
	{
		actorPosition.y = 956 - actorHeight / 2;
		isJumping = false;
		isFalling = false;
		allowRotation = false;
	}
	//Prevent player from going above play area
	if (static_cast<int>(actorPosition.y) <= 0)
	{
		actorPosition.y = actorHeight/2;
		jumpPower = 0; delta.y = 3;
		isFalling = true, isJumping = false;
	}

}

void Player::drawActor(SDL_Renderer* mRenderer, int index)
{
	//Player flap animation
	if (index == 0 && isAlive == true)
		hoverSpeed = 0.5;
	else if (index == 1 && isAlive == true)
		hoverSpeed = -0.5;
	else
	{
		hoverSpeed = 0.0f;//This prevents the player from rising when they die
		index = 0; //This stops the wings from flapping after death
	}



	//Updates the players position
	hitBox.x = static_cast<int>(actorPosition.x);
	hitBox.y = static_cast<int>(actorPosition.y - actorHeight / 2);

	//Show hitbox for debugging	
	//SDL_RenderDrawRect(mRenderer, &hitBox);
	SDL_RenderCopyEx(mRenderer, sprite[index], NULL, &hitBox, rotation, NULL, SDL_FLIP_NONE);
}

void Player::loadTexture(SDL_Renderer* mRenderer)
{
	SDL_DestroyTexture(sprite[0]);//Prevents memory leak
	SDL_DestroyTexture(sprite[1]);//Prevents memory leak
	spriteSurface[0] = IMG_Load("Assets/textures/frame-1.png");
	spriteSurface[1] = IMG_Load("Assets/textures/frame-2.png");
	if (spriteSurface[0] == nullptr || spriteSurface[1] == nullptr)
		SDL_Log("Texture failed to load: %s", SDL_GetError());
	else
	{
		sprite[0] = SDL_CreateTextureFromSurface(mRenderer, spriteSurface[0]);
		sprite[1] = SDL_CreateTextureFromSurface(mRenderer, spriteSurface[1]);
		if (spriteSurface[0] == nullptr || spriteSurface[1] == nullptr)
			throw runtime_error(SDL_GetError());
	}
	SDL_FreeSurface(spriteSurface[0]);
	SDL_FreeSurface(spriteSurface[1]);
}

Player::~Player()
{
	for (int i = 0; i < 2; i++)
	{
		SDL_DestroyTexture(sprite[i]);
		sprite[i] = nullptr;
		spriteSurface[i] = nullptr;
	}
}

void Player::flap()
{
	actorPosition.y += hoverSpeed;
}

int Player::getWidth()
{
	return actorWidth;
}

int Player::getHeight()
{
	return actorHeight;
}

//Returns the hitbox for collision detection
SDL_Rect Player::getHitBox()
{
	return hitBox;
}

//Returns the current x position of the actor
int Player::getPositionX()
{
	return static_cast<int>(actorPosition.x);
}

//Returns the current y position of the actor
int Player::getPositionY()
{
	return hitBox.y;
}

void Player::restartGame()
{
	actorPosition.x = 100.0f;
	actorPosition.y = 500;
	rotation = 0;
	isAlive = true;
	isJumping = false;
	isFalling = false;
	allowRotation = true;

}