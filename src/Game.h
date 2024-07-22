#pragma once

#include <SDL2-2.30.5/SDL.h>
#include <SDL2-2.30.5/SDL_ttf.h>
#include <SDL2-2.30.5/SDL_image.h>
#include <SDL2-2.30.5/SDL_mixer.h>

#include <vector>

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
	void UnloadData() const;
	void restartGame();
	static float deltaTime;
	static int SCREEN_X;
	static int SCREEN_Y;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool hasScrolled;
	bool deathSoundPlayed;

	std::vector<struct SoundEffect *> mSoundEffects;
	std::vector<class Text *> mText;
	std::vector<class Text *> tempText;

	// Following text instances are program specific
	struct Background *mainBG;
	struct SoundEffect *pointSound, *deathSound, *mainBGMusic, *collideSound, *flapSound;
	class Text *startTextHeader, *startTextSub, *scoreText, *hScoreText, *restartText;
	class Player *player1;
	struct Level *lvl;
	struct IMG *deathScreen;

	float secondCounter;
	bool startGame;
	int index;
	bool gameStart;
	bool restart;
	bool gameEnd;
	int mSpeed;
	int score;
	int highScore;
	TTF_Font *theFont;
};