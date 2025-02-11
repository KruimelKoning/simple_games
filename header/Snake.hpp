#pragma once

#include "Text.hpp"

#include <vector>
#include <chrono>
#include <iostream>

#include <SDL2/SDL.h>

#define STILE_SIZE 128
#define BODY_SIZE 112
#define APPLE_SIZE 96
#define SOFFSET 8
#define SNAKE_OFFSET 8 + SOFFSET
#define APPLE_OFFSET 16 + SOFFSET

#define BOARD_SIZE 8

enum Direction
{
	LEFT, RIGHT, UP, DOWN
};

struct Cord
{
	int	x, y;
};

class Snake
{
private:
	bool				_isRunning = true, _buttonPressed = false;
	Direction			_dir;
	std::vector<Cord>	_body, _tiles;

	Cord			_apple;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;
public:
	Snake(SDL_Renderer* renderer, SDL_Window* window);
	~Snake();

	void	run();
	void	move();
	void	drawSnake();
	void	drawApple();
	bool	checkColision(Cord& next);
	void	clearBoard();
};