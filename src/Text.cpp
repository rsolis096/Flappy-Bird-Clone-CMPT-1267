#include "Text.h"

Text::Text(TTF_Font *theFont, std::string tx, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b)
{
    textColor = {r, g, b};
    outlineColor = {0, 0, 0};
    textBox.x = x;
    textBox.y = y;
    textBox.w = w;
    textBox.h = h;
    textureText = tx;
    newTexture = nullptr;
    outLineTexture = nullptr;
    outLineSurface = nullptr;
    textSurface = nullptr;
    outLineBox = {static_cast<int>(x) - 2, static_cast<int>(y) - 1, static_cast<int>(w) + 25, static_cast<int>(h) + 25};
    textChanged = true;
}
Text::~Text()
{
    SDL_DestroyTexture(newTexture);
    SDL_DestroyTexture(outLineTexture);
    textSurface = nullptr;
    outLineSurface = nullptr;
    newTexture = nullptr;
    outLineTexture = nullptr;
}

void Text::drawText(SDL_Renderer *mRenderer, TTF_Font *theFont)
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

void Text::updateText(std::string t)
{
    textChanged = true;
    textureText = t;
}