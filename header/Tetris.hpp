#pragma once

#include "Text.hpp"
#include "Tools.hpp"

#include <vector>
#include <chrono>
#include <random>

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

struct Tile
{
	bool		exists;
	SDL_Color	colour;
};


struct	Tetris
{
	int						score = 0, level = 0;
	Tile					board[BOARD_HEIGHT][BOARD_WIDTH]{};
	bool					isHolding = false, isRunning = true;
	Block					current, hold, blocks[7];
	uint8_t					index = 0 ;
	SDL_Rect				textRect, holdRect, nextRect;
	std::vector<uint8_t>	sequence;

	SDL_Window*		window;
	SDL_Renderer*	renderer;
};