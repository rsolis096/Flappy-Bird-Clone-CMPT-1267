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
	hoverSpeed = 0.1;
	velocity = 0.0f;

	//Texture Related
	fileName = fN;
	sprite[1] = nullptr;
	sprite[0] = nullptr;
	spriteSurface[1] = nullptr;
	spriteSurface[0] = nullptr;

	//Bool
	isAlive = true;
	isFalling = false;
	allowRotation = true;
}

void Player::Jump()
{
	isFalling = true;
	if (velocity > 0)
		velocity = -40.0f;
	else
		velocity += -40.0f;
}

//update positon of player
void Player::UpdateActor(float deltaTime)
{	

	//Gravity
	if(isFalling == true)
	{
		//ACTOR POSITION 0.0f = top of screen, increase this value to "fall".

		//Update position in terms of meters
		actorPosition.y += velocity * (Game::deltaTime) * 10.0f;

		//Adjust velocity based on impact of gravity
		velocity += 9.81f * Game::deltaTime * 10.0f;
	}

	//Prevent player from falling through the floor
	if (static_cast<int>(actorPosition.y + actorHeight / 2) >= 956)
	{
		actorPosition.y = 956 - actorHeight / 2;
		isFalling = false;
		allowRotation = false;
	}
	//Prevent player from going above play area
	if (static_cast<int>(actorPosition.y) <= 0)
	{
		actorPosition.y = actorHeight/2;
		isFalling = true ;
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
	isFalling = false;
	allowRotation = true;

}