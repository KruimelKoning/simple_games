#pragma once

#include "Text.hpp"
#include "Snake.hpp"
#include "Sequence.hpp"
#include "Tetris.hpp"

#include <iostream>

#include <SDL2/SDL.h>

#define BUTTON_WIDTH  324
#define BUTTON_HEIGHT 128

#define SCREEN_WIDTH  1260
#define SCREEN_HEIGHT 840

// static SDL_Window*		window = nullptr;
// static SDL_Renderer*	renderer = nullptr;

class	Simple
{
private:
	bool	_isRunning = true;

	SDL_Rect		_buttonSnake, _buttonSequence, _buttonTetris;
	SDL_Event		_e;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;

public:
	Simple();
	~Simple();

	void	run();
	void	drawMenu();
	void	startGame();
	bool	isButtonPressed(SDL_Rect button);

	class	InitFailed : public std::exception
	{
		public:
			const char*	what() const throw() { return "failed to initialize"; }
	};
};