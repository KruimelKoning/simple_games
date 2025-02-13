#pragma once

#include "Text.hpp"

#include <vector>
#include <random>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>


namespace	sequence
{
	#define TILE_SIZE 240
	#define TILE_OFFSET 8
	#define BOARD_SIZE 3
	#define BORDER_SIZE 16
	#define OFFSET BORDER_SIZE / 2

	void	run(SDL_Renderer* renderer, SDL_Window* window);
}

struct	Sequencee
{
	bool	isRunning = true, isWaiting = false;
	uint8_t	level = 1, boardSize;
	std::vector<SDL_Rect>	tiles, sequence;

	SDL_Rect		tile, textRect;
	SDL_Window*		window;
	SDL_Renderer*	renderer;

	SDL_Color	colors[9];
};
