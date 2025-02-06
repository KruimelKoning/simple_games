#pragma once

#include "Text.hpp"

#include <vector>
#include <chrono>
#include <iostream>

#include <SDL2/SDL.h>

#define TILE_SIZE 32
#define BODY_SIZE 28
#define APPLE_SIZE 24
#define BORDER_SIZE 2

#define BOARD_SIZE 8

static SDL_Color	red   = {200, 100,  50, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {50,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};


enum Direction
{
	LEFT, RIGHT, UP, DOWN
};

class Snake
{
private:
	bool					_isRunning = true, _buttonPressed = false;
	Direction				_dir;
	std::vector<SDL_Rect>	_body, _tiles;

	SDL_Rect		_apple;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;
public:
	Snake(SDL_Renderer* renderer, SDL_Window* window);
	~Snake();

	void	run();
	void	move();
	void	clearBoard();
};