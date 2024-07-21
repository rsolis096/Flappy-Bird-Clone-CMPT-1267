#pragma once
#include "Game.h"
#include <string>

class Text
{
public:
	Text(TTF_Font*, std::string, float, float , float, float, Uint8, Uint8, Uint8);
	~Text();
	void drawText(SDL_Renderer*, TTF_Font*);
	void updateText(std::string);
	bool textChanged;

protected:
	SDL_Color textColor;
	SDL_Color outlineColor;
	SDL_Rect textBox;
	SDL_Rect outLineBox;
	SDL_Surface* textSurface;
	SDL_Surface* outLineSurface;
	SDL_Texture* newTexture;
	SDL_Texture* outLineTexture;
	std::string textureText;
};

inline Text::Text(TTF_Font* theFont, std::string tx, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b)
{
	textColor = { r, g, b };
	outlineColor = {0,0,0};
	textBox.x = x;
	textBox.y = y;
	textBox.w = w;
	textBox.h = h;
	textureText = tx;
	newTexture = nullptr;
	outLineTexture = nullptr;
	outLineSurface = nullptr;
	textSurface = nullptr;
	outLineBox = { static_cast<int>(x)-2, static_cast<int>(y) - 1 ,static_cast<int>(w) + 25 ,static_cast<int>(h) + 25};
	textChanged = true;

}
inline Text::~Text()
{
	SDL_DestroyTexture (newTexture);
	SDL_DestroyTexture (outLineTexture);
	textSurface = nullptr;
	outLineSurface = nullptr;
	newTexture = nullptr;
	outLineTexture = nullptr;
}

inline void Text::drawText(SDL_Renderer* mRenderer, TTF_Font* theFont)
{
	if (textChanged)
	{

		if (newTexture != nullptr)
			SDL_DestroyTexture(newTexture);

		textSurface = TTF_RenderText_Solid(theFont, textureText.c_str(), textColor);
		newTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);

		textChanged = false;

		SDL_FreeSurface(textSurface);
	}

	SDL_RenderCopy(mRenderer, newTexture, NULL, &textBox);
}

inline void Text::updateText(std::string t)
{
	textChanged = true;
	textureText = t;
}