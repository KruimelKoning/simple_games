#include "../header/Sequence.hpp"

Sequence::Sequence(uint8_t boardSize) : _boardSize(boardSize)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw InitFailed();
	_window = SDL_CreateWindow("simple", 640, 128, _boardSize * TILE_SIZE, _boardSize * TILE_SIZE, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
		throw InitFailed();
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (_renderer == nullptr)
	{
		SDL_DestroyWindow(_window);
		throw InitFailed();
	}
	_tile   = (SDL_Rect){0, 0, TILE_SIZE - BORDER_SIZE, TILE_SIZE - BORDER_SIZE};
	_border = (SDL_Rect){0, 0, TILE_SIZE, TILE_SIZE};
	
	for (int y = 0; y < _boardSize; y++)
	{
		for (int x = 0; x < _boardSize; x++)
			_sequence.push_back({x, y});
	}
	int i = 0;
	for (; i < 3; i++)
		_colors[i] = {50, (uint8_t)(200 - (i * 40)), 50, 255};
	for (; i < 6; i++)
		_colors[i] = {50, 100, (uint8_t)(200 - (i * 20)), 255};
	for (; i < 9; i++)
		_colors[i] = {(uint8_t)(255 - (i * 40)), 50, 50, 255};
}

Sequence::~Sequence()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit(); // always call this function
}

void	Sequence::drawBorders()
{
	for (int y = 0; y < _boardSize; y++)
		for (int x = 0; x < _boardSize; x++)
		{
			_border.x = x * TILE_SIZE;
			_border.y = y * TILE_SIZE;
			SDL_SetRenderDrawColor(_renderer, _black.r, _black.g, _black.b, _black.a);
			SDL_RenderFillRect(_renderer, &_border);
		}
	SDL_RenderPresent(_renderer);
}

void	Sequence::clearBoard()
{
	for (int y = 0; y < _boardSize; y++)
		for (int x = 0; x < _boardSize; x++)
		{
			_tile.x = x * TILE_SIZE + OFFSET;
			_tile.y = y * TILE_SIZE + OFFSET;
			SDL_SetRenderDrawColor(_renderer, _grey.r, _grey.g, _grey.b, _grey.a);
			SDL_RenderFillRect(_renderer, &_tile);
		}
	SDL_RenderPresent(_renderer);
}

void	Sequence::drawTile(SDL_Color colour)
{
	SDL_SetRenderDrawColor(_renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(_renderer, &_tile);
	SDL_RenderPresent(_renderer);
}

void	Sequence::showSequence()
{
	for (int i = 0; i < _level; i++)
	{
		// std::cout << "sequence[" << i << "] x: " << _sequence[i].x << " sequence[" << i << "] y: "  << _sequence[i].y << std::endl;
		_tile.x = _sequence[i].x * TILE_SIZE + OFFSET;
		_tile.y = _sequence[i].y * TILE_SIZE + OFFSET;
		// std::cout << "square x: " << _tile.x << " square y: " << _tile.y << std::endl;
		// std::cout << "square w: " << _tile.w << " square h: " << _tile.h << std::endl;
		drawTile(_colors[_level - 1]);
		SDL_Delay(1000);
		drawTile(_grey);
	}
	_isWaiting = true;
}

bool	Sequence::makeMove(int& i, vector2 mouseInput)
{
	// std::cout << "mouse x: " << mouseInput.x << " mouse y: " << mouseInput.y << std::endl;
	// std::cout << "sequence[" << i << "] x: " << _sequence[i].x << " sequence[" << i << "] y: "  << _sequence[i].y << std::endl;
	if (_sequence[i].x != mouseInput.x || _sequence[i].y != mouseInput.y)
		return std::cout << "YOU LOSE!\n", false;

	_tile.x = _sequence[i].x * TILE_SIZE + OFFSET;
	_tile.y = _sequence[i].y * TILE_SIZE + OFFSET;

	drawTile(_colors[_level - 1]);
	i++;
	if (i == _level)
	{
		SDL_Delay(800);
		clearBoard();
		SDL_Delay(500);
		_level++;
		_isWaiting = false;
		i = 0;
	}
	return true;
}

void	Sequence::run()
{
	SDL_Event			e;
	std::random_device	rd;
	std::mt19937		g(rd());
	std::shuffle(_sequence.begin(), _sequence.end(), g);

	drawBorders();
	clearBoard();

	int	i = 0;
	while (_isRunning && _level <= _boardSize * _boardSize)
	{
		if (_isWaiting == false)
			showSequence();
		while (SDL_PollEvent(&e))
		{
			
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				_isRunning = false;
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (makeMove(i, {e.button.x / TILE_SIZE, e.button.y / TILE_SIZE}) == false)
						_isRunning = false;
				}
			}
		}
	}
}