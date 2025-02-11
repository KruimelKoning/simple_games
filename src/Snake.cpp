#include "../header/Snake.hpp"

static SDL_Color	red   = {200, 100,  80, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {80,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};

Snake::Snake(SDL_Renderer* renderer, SDL_Window* window) : _renderer(renderer), _window(window), _dir(UP)
{
	SDL_SetWindowSize(_window, BOARD_SIZE * (STILE_SIZE + SOFFSET) + SOFFSET, BOARD_SIZE * (STILE_SIZE + SOFFSET) + SOFFSET);
	srand(time(nullptr));

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
			_tiles.push_back({x, y});
	}
	_apple = {1 ,1};
	_body.push_back({4, 4});
	SDL_SetRenderDrawColor(_renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(_renderer);
	clearBoard();
	drawSnake();
	drawApple();
	SDL_RenderPresent(_renderer);
}

Snake::~Snake()
{
	std::cout << "CU\n";
}

void	Snake::clearBoard()
{
	SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
	for (const Cord& tile : _tiles)
	{
		SDL_Rect	rect = {tile.x * (STILE_SIZE + SOFFSET) + SOFFSET, tile.y * (STILE_SIZE + SOFFSET) + SOFFSET, STILE_SIZE, STILE_SIZE};
		SDL_RenderFillRect(_renderer, &rect);
	}
	SDL_RenderPresent(_renderer);
}

void	Snake::drawApple()
{
	SDL_Rect	rect = {_apple.x * (STILE_SIZE + SOFFSET) + APPLE_OFFSET, _apple.y * (STILE_SIZE + SOFFSET) + APPLE_OFFSET, APPLE_SIZE, APPLE_SIZE};
	SDL_SetRenderDrawColor(_renderer, red.r, red.g, red.b, red.a);
	SDL_RenderFillRect(_renderer, &rect);
	SDL_RenderPresent(_renderer);
}

void	Snake::drawSnake()
{
	std::cout << "size: " << _body.size() << std::endl;
	Cord	&head = _body.back(), &tail = _body[0];


	SDL_Rect	rect = {head.x * (STILE_SIZE + SOFFSET) + SNAKE_OFFSET, head.y * (STILE_SIZE + SOFFSET) + SNAKE_OFFSET, BODY_SIZE, BODY_SIZE};
	// std::cout << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h << std::endl;
	SDL_SetRenderDrawColor(_renderer, green.r, green.g, green.b, green.a);
	SDL_RenderFillRect(_renderer, &rect);
	if (_body.size() > 1 && (head.x != _apple.x || head.y != _apple.y))
	{
		rect = {tail.x * (STILE_SIZE + SOFFSET) + SOFFSET, tail.y * (STILE_SIZE + SOFFSET) + SOFFSET, STILE_SIZE, STILE_SIZE};
		// std::cout << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h << std::endl;
		SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
		SDL_RenderFillRect(_renderer, &rect);
		_body.erase(_body.begin());
	}

	SDL_RenderPresent(_renderer);
}

bool	Snake::checkColision(Cord& next)
{
	for (const Cord& cord : _body)
	{
		if (cord.x == next.x && cord.y == next.y)
			return true;
	}
	return next.x >= BOARD_SIZE || next.x < 0 || next.y >= BOARD_SIZE || next.y < 0;
}

void	Snake::move()
{
	Cord	next = _body.back();
	switch (_dir)
	{
		case UP:    next.y--; break;
		case DOWN:  next.y++; break;
		case LEFT:  next.x--; break;
		case RIGHT: next.x++; break;
	}
	if (checkColision(next) == true)
	{
		std::cout << "You lose bro" << std::endl;
		_isRunning = false;
		return ;
	}
	_body.push_back({next.x, next.y});
	drawSnake();
	if (next.x == _apple.x && next.y == _apple.y)
	{
		_apple.x = rand() % BOARD_SIZE;
		_apple.y = rand() % BOARD_SIZE;
		drawApple();
	}
}

void Snake::run()
{
	SDL_Event	e;
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
					case SDLK_UP:    _dir = UP;    break;
					case SDLK_DOWN:  _dir = DOWN;  break;
					case SDLK_LEFT:  _dir = LEFT;  break;
					case SDLK_RIGHT: _dir = RIGHT; break;
				}
				_buttonPressed = true;
			}
		}

	}
}