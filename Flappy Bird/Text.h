#pragma once
#include "Game.h"

class Text
{
public:
	Text(TTF_Font*, string, int, int, int, int, Uint8, Uint8, Uint8);
	~Text();
	void drawText(SDL_Renderer*, TTF_Font*);
	void updateText(string);

protected:
	SDL_Color textColor;
	SDL_Color outlineColor;
	SDL_Rect textBox;
	SDL_Rect outLineBox;
	SDL_Surface* textSurface;
	SDL_Surface* outLineSurface;
	SDL_Texture* newTexture;
	SDL_Texture* outLineTexture;
	string textureText;
};

Text::Text(TTF_Font* theFont, string tx, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
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
	outLineBox = { x-2, y-1 ,w + 10,h + 10 };

}
Text::~Text()
{
	SDL_DestroyTexture (newTexture);
	SDL_DestroyTexture (outLineTexture);
	textSurface = nullptr;
	outLineSurface = nullptr;
	newTexture = nullptr;
	outLineTexture = nullptr;
}

void Text::drawText(SDL_Renderer* mRenderer, TTF_Font* theFont)
{
	
	//For the main Text
	textSurface = TTF_RenderText_Solid(theFont, textureText.c_str(), textColor);
	newTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);

	//For the outline of the text
	//SDL_DestroyTexture(outLineTexture);
	//outLineSurface = TTF_RenderText_Solid(theFont, textureText.c_str(), outlineColor);
	//outLineTexture = SDL_CreateTextureFromSurface(mRenderer, outLineSurface);

	//SDL_RenderCopy(mRenderer, outLineTexture, NULL, &outLineBox);
	SDL_RenderCopy(mRenderer, newTexture, NULL, &textBox);
		
	SDL_DestroyTexture(newTexture);//Destroy the previous text texture
	SDL_FreeSurface(textSurface);
	
}

void Text::updateText(string t)
{
	textureText = t;
}