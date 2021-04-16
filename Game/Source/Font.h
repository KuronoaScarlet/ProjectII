#ifndef __FONT_H__
#define __FONT_H__

#include <string.h>
#include "List.h"
#include "Module.h"
#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"

class FontsModule : public Module
{
public:
	List<TTF_Font*> fonts;
	TTF_Font* font1;

	//tmp copy
	//SDL_Renderer* renderer;

	FontsModule();
	~FontsModule();
	TTF_Font* const AddFont(const char* path);
	SDL_Texture* DrawFont(SDL_Renderer* renderer, int index, SDL_Color color, const char* text);
};

FontsModule::FontsModule()
{
	
	font1 = AddFont("Assets/Fonts/Kurale-Regular.ttf");
}

FontsModule::~FontsModule()
{
	fonts.Clear();
}

TTF_Font* const FontsModule::AddFont(const char* path)
{
	TTF_Font* font = TTF_OpenFont(path, 32);
	return font;
}

SDL_Texture* FontsModule::DrawFont(SDL_Renderer* renderer, int index, SDL_Color color, const char* text)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font1/*fonts[index]*/, text, color);//TTF_RenderText_Shaded
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	//=========================NEED FIX THIS RECT
	int w = 10 * strlen(text);
	SDL_Rect r{ 200,200,w,30 };
	SDL_RenderCopy(renderer, textTexture, 0, &r);

	//debug box
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &r);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	return textTexture;
}

#endif
