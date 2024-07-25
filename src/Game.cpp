#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "Text.h"
#include "Level.h"
#include "Img.h"
#include "SoundEffect.h"

float Game::deltaTime = 0.0f;
float timeSpacePressed = 0.0f;

int Game::SCREEN_Y = 544;
int Game::SCREEN_X = 960;

Game::Game()
{
	index = 0;
	deltaTime = 0;
	secondCounter = 0;
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;
	mTicksCount = 0;
	player1 = nullptr;
	mainBG = nullptr;
	hasScrolled = false;
	scoreText = nullptr;
	startTextHeader = nullptr;
	startTextSub = nullptr;
	gameStart = false;
	gameEnd = false;
	mSpeed = 0;
	score = 0;
	highScore = 0;
	pointSound = nullptr;
	deathSound = nullptr;
	deathSoundPlayed = false;
	collideSound = nullptr;
	flapSound = 0;
	restart = false;
	restartText = nullptr;
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Open Audio
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048);

	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Flappy Bird", // Window title
		0,			   // Top left x-coordinate of window
		25,	   // Top left y-coordinate of window
		SCREEN_X,	   // Width of window
		SCREEN_Y,	   // Height of window
		0			   // Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init())
	{
		SDL_Log("Unable to initialize SDL_font: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::LoadData()
{
	player1 = new Player("Assets/textures/frame-1.png", 0.08 * SCREEN_Y, 0.08 * SCREEN_Y);
	player1->loadTexture(mRenderer);

	theFont = TTF_OpenFont("Assets/font/SuperMario256.ttf", 512);

	scoreText = new Text(theFont, " ", 5, 20, SCREEN_Y * 0.2, 0.05 * SCREEN_X, 255, 255, 255);
	hScoreText = new Text(theFont, "Best: 0", SCREEN_X * 0.75, 20, SCREEN_Y * 0.2, 0.05 * SCREEN_X, 255, 255, 255);
	startTextHeader = new Text(theFont, "FLAPPY BIRD", SCREEN_X * 0.35, SCREEN_Y * 0.4, SCREEN_X * 0.3, SCREEN_Y * 0.1, 255, 255, 255);
	startTextSub = new Text(theFont, "PRESS SPACE TO START", SCREEN_X * 0.25, SCREEN_Y * 0.5, SCREEN_X * 0.5, SCREEN_Y * 0.1, 255, 255, 255);
	restartText = new Text(theFont, "Press R To Restart", SCREEN_X * 0.2, SCREEN_Y * 0.8, SCREEN_X * 0.6, SCREEN_Y * 0.05, 255, 255, 255);
	mText.emplace_back(scoreText);
	mText.emplace_back(hScoreText);
	tempText.emplace_back(startTextHeader);
	tempText.emplace_back(startTextSub);
	tempText.emplace_back(restartText);

	mainBG = new Background;
	mainBG->loadBackground(mRenderer, 0);
	mainBG->loadBackground(mRenderer, 1);

	lvl = new Level(mRenderer);
	deathScreen = new IMG("Assets/textures/death.png", 0, SCREEN_Y/2, SCREEN_X, SCREEN_Y * 0.20);

	pointSound = new SoundEffect("Assets/sounds/point.wav", 1, 0, 15);
	deathSound = new SoundEffect("Assets/sounds/death.wav", 2, 0, 18);
	mainBGMusic = new SoundEffect("Assets/sounds/music.wav", 3, -1, 13);
	collideSound = new SoundEffect("Assets/sounds/collision.wav", 4, 0, 15);
	flapSound = new SoundEffect("Assets/sounds/flap.wav", 5, 0, 15);


	mSoundEffects.push_back(pointSound);
	mSoundEffects.push_back(deathSound);
	mSoundEffects.push_back(mainBGMusic);
	mSoundEffects.push_back(collideSound);
	mSoundEffects.push_back(flapSound);

	mainBGMusic->playSound();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	bool isSpacePressed = false;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Used to exit the start screen and play flap noise when space is pressed
	if (state[SDL_SCANCODE_SPACE] && !gameEnd)
	{
		// cout << SDL_GetTicks() - timeSpacePressed << endl;

		if (SDL_GetTicks() - timeSpacePressed > 300) // Prevents holding down the space and flying like a rocket
		{
			timeSpacePressed = SDL_GetTicks();
			gameStart = true;
			mSpeed = 2;
			flapSound->playSound();
			player1->Jump();
		}
	}

	if (state[SDL_SCANCODE_R])
	{
		if (gameEnd == true)
			restart = true;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		; // SDL DELAY hurt performance, this is best solution for 60fps lock

	// Delta time is the difference in ticks from last frame (converted to seconds)
	deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update Player
	player1->UpdateActor(deltaTime);

	// Detect collision between player and top pipes
	
	SDL_Rect hitBox = player1->getHitBox();
	for (auto& element : lvl->topObjects)
		if (SDL_HasIntersection(&element, &hitBox))
		{
			player1->isAlive = false;
			gameEnd = true;
			mSpeed = 0;
		}

	// Detect collision between player and bottom pipes
	for (auto& element : lvl->bottomObjects)
		if (SDL_HasIntersection(&element, &hitBox))
		{
			player1->isAlive = false;
			gameEnd = true;
			mSpeed = 0;
		}
	
	// Detect when a player passes in between pipes
	for (auto& element : lvl->pointObject)
	{
		if (player1->getPositionX() >= static_cast<int>(element.x + element.w) && player1->getPositionX() < static_cast<int>(element.x + element.w + 5))
		{
			score++;
			pointSound->playSound();
		}
	}

	// Kills the player when they hit the ground
	if (player1->getPositionY() >= 0.8 * SCREEN_Y)
	{
		player1->isAlive = false;
		gameEnd = true;
		mSpeed = 0;
	}

	// Scrolling background
	if (mSpeed != 0)
	{
		lvl->updatePosition(mSpeed + 1.5);	
		mainBG->updatePosition(mSpeed + 0.5); 
	}
	else
	{
		lvl->updatePosition(0);	   
		mainBG->updatePosition(0); 
	}
	player1->flap(); // Makes the bird hover! NEAT. Uses the draw function

	if (restart == true)
		restartGame();
}

void Game::GenerateOutput()
{
	// Draw Background
	SDL_RenderClear(mRenderer);
	mainBG->drawBackground(mRenderer);

	// Draw Level
	lvl->drawLevel(mRenderer);

	// Draw player, this timer allows to bird to flap its wings
	secondCounter += deltaTime;
	if (secondCounter >= 0.25)
	{
		secondCounter = 0.0f;
		if (index == 0)
			index = 1;
		else
			index = 0;
	}
	// Makes the player rotate forwards when they die
	if (gameEnd == true && secondCounter >= 0.02 && player1->allowRotation == true)
	{
		player1->rotation += 1.5;
	}

	player1->drawActor(mRenderer, index);

	// Show Death Screen overtop everything
	if (gameEnd == true)
	{
		deathScreen->drawFadeIn(mRenderer, deltaTime);
		restartText->drawText(mRenderer, theFont);
	}

	// Display start menu over top everything
	if (gameStart == false)
	{
		startTextHeader->drawText(mRenderer, theFont);
		startTextSub->drawText(mRenderer, theFont);
	}

	// Draw text (not including splash screen text)
	scoreText->updateText("Score: " + std::to_string(score));
	for (auto element : mText)
	{
		element->drawText(mRenderer, theFont);
	}
	if (deathSoundPlayed == false && gameEnd == true)
	{
		deathSound->playSound();
		collideSound->playSound();
		deathSoundPlayed = true;
		mainBGMusic->stopSound();
	}
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	TTF_CloseFont(theFont);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::UnloadData() const
{
	for (auto& texts : mText)
		delete texts;
	for (auto& texts : tempText)
		delete texts;
	for (auto &sounds : mSoundEffects)
		delete sounds;
	delete player1;
	delete mainBG;
	delete deathScreen;
	delete lvl;
}

void Game::restartGame()
{
	gameStart = false;
	gameEnd = false;
	restart = false;
	deathSoundPlayed = false;
	deathScreen->setAlpha(0);
	mSpeed = 0;
	if (score > highScore)
	{
		highScore = score;
		hScoreText->updateText("Best : " + std::to_string(score));
	}
	score = 0;
	player1->restartGame();
	mainBG->restartGame();
	lvl->restartGame();
	mainBGMusic->resumeSound();
}