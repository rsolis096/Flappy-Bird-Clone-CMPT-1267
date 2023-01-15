#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"

#include <vector>
#include <string>

#include <iostream>
using namespace std;

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void LoadData();
	void UnloadData();
	void restartGame();
	float deltaTime;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;	
	bool mIsRunning;
	bool hasScrolled;
	bool deathSoundPlayed;

	vector<struct SoundEffect*> mSoundEffects;
	vector<class Text*> mText;
	vector<class Text*> tempText;

	//Following text instances are program specific
	struct Background* mainBG;
	struct SoundEffect* pointSound, *deathSound,* mainBGMusic, *collideSound, *flapSound;
	class Text* startTextHeader, *startTextSub, * scoreText,*hScoreText, *restartText;
	class Player* player1;
	struct Level* lvl;
	struct IMG* deathScreen;

	float secondCounter;
	bool startGame;
	int index;
	bool gameStart;
	bool restart;
	bool gameEnd;
	int mSpeed;
	int score;
	int highScore;
	TTF_Font* theFont;
};