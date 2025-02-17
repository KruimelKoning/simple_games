#pragma once

#include "Tools.hpp"

#include <vector>
#include <chrono>

#include <SDL2/SDL.h>

namespace	tetris
{
	#define OFFSET       4
	#define TILE_SIZE    64
	#define BORDER_SIZE  2
	#define BOARD_WIDTH  10
	#define BOARD_HEIGHT 16

	void	run(SDL_Renderer* renderer, SDL_Window* window);
};

enum	blocks
{
	YELLOW, LIGHT_BLUE, DARK_BLUE, RED, GREEN, ORANGE, PURPLE
};

struct	Block
{
	SDL_Color			colour;
	std::vector<Cord>	cords;
};

struct	Tetris
{
	bool				board[BOARD_HEIGHT][BOARD_WIDTH]{};
	bool				isRunning = true;
	SDL_Color			colours[7];
	std::vector<Cord>	current, blocks[7];

	SDL_Window*		window;
	SDL_Renderer*	renderer;
};