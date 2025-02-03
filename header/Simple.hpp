#pragma once

#include "Sequence.hpp"
#include "Text.hpp"

#include <iostream>

#include <SDL2/SDL.h>

#define BUTTON_WIDTH  256
#define BUTTON_HEIGHT 128

#define SCREEN_WIDTH  1260
#define SCREEN_HEIGHT 840

class	Simple
{
private:
	bool	_isRunning = true;

	SDL_Rect		_button;
	SDL_Event		_e;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;

public:
	Simple();
	~Simple();

	void	run();
	void	drawMenu();
	void	startGame();
	bool	isButtonPressed();

	class	InitFailed : public std::exception
	{
		public:
			const char*	what() const throw() { return "failed to initialize"; }
	};
};