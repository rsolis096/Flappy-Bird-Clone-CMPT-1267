#pragma once
#include "Game.h"
#include <string>

class Text
{
public:
	Text(TTF_Font *, std::string, float, float, float, float, Uint8, Uint8, Uint8);
	~Text();
	void drawText(SDL_Renderer *, TTF_Font *);
	void updateText(std::string);
	bool textChanged;

protected:
	SDL_Color textColor;
	SDL_Color outlineColor;
	SDL_Rect textBox;
	SDL_Rect outLineBox;
	SDL_Surface *textSurface;
	SDL_Surface *outLineSurface;
	SDL_Texture *newTexture;
	SDL_Texture *outLineTexture;
	std::string textureText;
};




