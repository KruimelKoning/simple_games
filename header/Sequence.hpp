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

class Sequence
{
private:
	bool	_isRunning = true, _isWaiting = false;
	uint8_t	_level = 1, _boardSize;
	std::vector<SDL_Rect>	_tiles, _sequence;

	SDL_Rect		_tile, _textRect;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;

	SDL_Color	_colors[9];
public:
	Sequence(SDL_Renderer* renderer, SDL_Window* window, uint8_t boardSize = 3);
	~Sequence();

	void	run();
	void	drawBorders();
	void	clearBoard();
	void	drawTile(SDL_Color colour, SDL_Rect& tile);

	void	showSequence();
	bool	makeMove(int& i, SDL_Point mouseInput);

	class	InitFailed : public std::exception
	{
		public:
			const char*	what() const throw()
			{
				return "failed to initialize";
			}
	};
};
