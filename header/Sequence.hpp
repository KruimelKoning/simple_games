#pragma once

#include <vector>
#include <random>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>

#define TILE_SIZE 256
#define BORDER_SIZE 32
#define OFFSET BORDER_SIZE / 2

struct vector2
{
	int	x, y;
};

union rgb
{
	uint32_t	colour;
	uint8_t		val[4];
};

class Sequence
{
private:
	bool	_isRunning = true, _isWaiting = false;
	uint8_t	_level = 1, _boardSize;
	std::vector<vector2>		_sequence;

	SDL_Rect		_tile, _border;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;

	SDL_Color	_colors[9];
	SDL_Color	_red   = {200, 50, 50, 255};
	SDL_Color	_grey  = {128, 128, 128, 255};
	SDL_Color	_blue  = {50, 100, 200, 255};
	SDL_Color	_green = {50, 200, 100, 255};
	SDL_Color	_black = {200, 200, 200, 255};
public:
	Sequence(uint8_t boardSize = 3);
	~Sequence();

	void	run();
	void	drawBorders();
	void	clearBoard();
	void	drawTile(SDL_Color colour);

	void	showSequence();
	bool	makeMove(int& i, vector2 mouseInput);

	class	InitFailed : public std::exception
	{
		public:
			const char*	what() const throw()
			{
				return "failed to initialize";
			}
	};
};
