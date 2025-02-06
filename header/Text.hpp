#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
#include <iostream>
#include <unordered_map>

#define FNT_PATH "./Text/TestFont.fnt"
#define FNT_TEXTURE_PATH "./Text/TestFont_0.png"

struct Token
{
	int			xoffset, yoffset, xadvance;
	SDL_Rect	srcRect;
};

static SDL_Texture*	fontTexture = nullptr;
static std::unordered_map<int, Token>	tokenMap;

bool	loadFontBM(SDL_Renderer* renderer);
void	renderCenteredText(SDL_Renderer* renderer, const std::string& text, SDL_Rect rect);
void	renderText(SDL_Renderer* renderer, const std::string& text, int x, int y);