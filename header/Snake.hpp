#pragma once

#include "Text.hpp"
#include "Tools.hpp"

#include <vector>
#include <chrono>
#include <iostream>

#include <SDL2/SDL.h>

namespace	snake
{
	#define TILE_SIZE 128
	#define BODY_SIZE 112
	#define APPLE_SIZE 96
	#define SOFFSET 8
	#define SNAKE_OFFSET 8 + SOFFSET
	#define APPLE_OFFSET 16 + SOFFSET
	#define BOARD_SIZE 8

	void	run(SDL_Renderer* renderer, SDL_Window* window);
};

struct	Snake
{
	Cord				apple;
	bool				isRunning = true, buttonPressed = false;
	Direction			dir;
	std::vector<Cord>	body, tiles;

	SDL_Window*		window;
	SDL_Renderer*	renderer;
};