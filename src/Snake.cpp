#include "../header/Snake.hpp"

Snake::Snake(SDL_Renderer* renderer, SDL_Window* window) : _renderer(renderer), _window(window), _dir(UP)
{
	SDL_SetWindowSize(_window, BOARD_SIZE * (TILE_SIZE + BORDER_SIZE), BOARD_SIZE * (TILE_SIZE + BORDER_SIZE));

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
			_tiles.push_back({x * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, y * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE, TILE_SIZE});
	}
	SDL_SetRenderDrawColor(_renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(_renderer);
}

Snake::~Snake()
{
	std::cout << "CU\n";
}

void	Snake::clearBoard()
{
	SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
	for (SDL_Rect& tile : _tiles)
		SDL_RenderFillRect(_renderer, &tile);
	SDL_RenderPresent(_renderer);

}

void	Snake::move()
{

}

void Snake::run()
{
	SDL_Event	e;
	clearBoard();
	auto	start = std::chrono::high_resolution_clock::now();

	while (_isRunning)
	{
		auto	now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = now - start;
		if (elapsed.count() >= 1.0)
		{
			move();
			start = now;
			_buttonPressed = false;
		}
		while (SDL_PollEvent(&e))
		{
			
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				_isRunning = false;
			if (e.type == SDL_KEYDOWN && !_buttonPressed)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:    _dir = UP;
					case SDLK_DOWN:  _dir = DOWN;
					case SDLK_LEFT:  _dir = LEFT;
					case SDLK_RIGHT: _dir = RIGHT;
				}
				_buttonPressed = true;
			}
		}

	}
}