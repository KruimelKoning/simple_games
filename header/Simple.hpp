#pragma once

#include <iostream>

#include <SDL2/SDL.h>

class	Simple
{
private:
	bool	_isRunning;

	SDL_Rect		_button;
	SDL_Window*		_window;
	SDL_Renderer*	_renderer;

public:
	Simple();
	~Simple();

	void	run();

	class	InitFailed : public std::exception
	{
		public:
			const char*	what() const throw() { return "failed to initialize"; }
	};
};