#include "../header/Sequence.hpp"

static SDL_Color	red   = {200, 100,  50, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {50,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};

Sequence::Sequence(SDL_Renderer* renderer, SDL_Window* window, uint8_t boardSize) : _boardSize(boardSize), _renderer(renderer), _window(window)
{
	if (boardSize > 5)
		throw InitFailed();
	SDL_SetWindowSize(_window, (_boardSize + 2) * (TILE_SIZE + TILE_OFFSET * 2), _boardSize * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET);

	_tile = {0, 0, TILE_SIZE - BORDER_SIZE, TILE_SIZE - BORDER_SIZE};
	_textRect = {TILE_SIZE * (_boardSize + 1) - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE, TILE_SIZE};

	for (int y = 0; y < _boardSize; y++)
	{
		for (int x = 0; x < _boardSize; x++)
			_tiles.push_back({x * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET, y * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET, TILE_SIZE, TILE_SIZE});
	}
	int i = 0;
	for (; i < 3; i++)
		_colors[i] = {50, (uint8_t)(200 - (i * 40)), 50, 255};
	for (; i < 6; i++)
		_colors[i] = {50, 100, (uint8_t)(200 - (i * 20)), 255};
	for (; i < 9; i++)
		_colors[i] = {(uint8_t)(200 - (i * 40)), 50, 50, 255};
	srand(time(nullptr));
}

Sequence::~Sequence()
{
	std::cout << "Later dude\n";
}

void	Sequence::drawBorders()
{
	SDL_SetRenderDrawColor(_renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);
}

void	Sequence::clearBoard()
{
	SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
	for (SDL_Rect& tile : _tiles)
		SDL_RenderFillRect(_renderer, &tile);
	SDL_RenderPresent(_renderer);
}

void	Sequence::drawTile(SDL_Color colour, SDL_Rect& tile)
{
	SDL_SetRenderDrawColor(_renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(_renderer, &tile);
	SDL_RenderPresent(_renderer);
}

void	Sequence::showSequence()
{
	SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
	SDL_RenderFillRect(_renderer, &_textRect);
	renderCenteredText(_renderer, "Level: " + std::to_string(_level), _textRect);
	SDL_RenderPresent(_renderer);

	_sequence.push_back(_tiles[rand() % 9]);
	for (SDL_Rect& tile : _sequence)
	{
		drawTile(_colors[_level - 1], tile);
		SDL_Delay(800);
		drawTile(grey, tile);
		SDL_Delay(200);
	}
	_isWaiting = true;
}

bool	Sequence::makeMove(int& i, SDL_Point mouseInput)
{
	// std::cout << "mouse x: " << mouseInput.x << " mouse y: " << mouseInput.y << std::endl;
	// std::cout << "sequence[" << i << "] x: " << _tiles[i].x << " sequence[" << i << "] y: "  << _tiles[i].y << std::endl;
	if (SDL_PointInRect(&mouseInput, &_sequence[i]) == false)
		return std::cout << "YOU LOSE!\n", false;

	drawTile(_colors[_level - 1], _sequence[i]);
	SDL_Delay(400);
	drawTile(grey, _sequence[i]);
	i++;
	if (i == _level)
	{
		SDL_Delay(800);
		_level++;
		_isWaiting = false;
		i = 0;
	}
	return true;
}

void	Sequence::run()
{
	SDL_Event	_e;
	drawBorders();
	clearBoard();

	int	i = 0;
	while (_isRunning && _level <= _boardSize * _boardSize)
	{
		if (_isWaiting == false)
			showSequence();
		while (SDL_PollEvent(&_e))
		{
			
			if (_e.type == SDL_QUIT || (_e.type == SDL_KEYDOWN && _e.key.keysym.sym == SDLK_ESCAPE))
				_isRunning = false;
			if (_e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (_e.button.button == SDL_BUTTON_LEFT)
				{
					if (makeMove(i, {_e.button.x, _e.button.y}) == false)
						_isRunning = false;
				}
			}
		}
	}
}