#include "../header/Snake.hpp"

using namespace snake;

static SDL_Color	red   = {200, 100,  80, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {80,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};

void	clearBoard(Snake& s)
{
	SDL_SetRenderDrawColor(s.renderer, grey.r, grey.g, grey.b, grey.a);
	for (const Cord& tile : s.tiles)
	{
		SDL_Rect	rect = {tile.x * (TILE_SIZE + SOFFSET) + SOFFSET, tile.y * (TILE_SIZE + SOFFSET) + SOFFSET, TILE_SIZE, TILE_SIZE};
		SDL_RenderFillRect(s.renderer, &rect);
	}
	SDL_RenderPresent(s.renderer);
}

void	drawApple(Snake& s)
{
	SDL_Rect	rect = {s.apple.x * (TILE_SIZE + SOFFSET) + APPLE_OFFSET, s.apple.y * (TILE_SIZE + SOFFSET) + APPLE_OFFSET, APPLE_SIZE, APPLE_SIZE};
	SDL_SetRenderDrawColor(s.renderer, red.r, red.g, red.b, red.a);
	SDL_RenderFillRect(s.renderer, &rect);
	SDL_RenderPresent(s.renderer);
}

void	drawSnake(Snake& s)
{
	std::cout << "size: " << s.body.size() << std::endl;
	Cord	&head = s.body.back(), &tail = s.body[0];


	SDL_Rect	rect = {head.x * (TILE_SIZE + SOFFSET) + SNAKE_OFFSET, head.y * (TILE_SIZE + SOFFSET) + SNAKE_OFFSET, BODY_SIZE, BODY_SIZE};
	// std::cout << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h << std::endl;
	SDL_SetRenderDrawColor(s.renderer, green.r, green.g, green.b, green.a);
	SDL_RenderFillRect(s.renderer, &rect);
	if (s.body.size() > 1 && (head.x != s.apple.x || head.y != s.apple.y))
	{
		rect = {tail.x * (TILE_SIZE + SOFFSET) + SOFFSET, tail.y * (TILE_SIZE + SOFFSET) + SOFFSET, TILE_SIZE, TILE_SIZE};
		// std::cout << "x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h << std::endl;
		SDL_SetRenderDrawColor(s.renderer, grey.r, grey.g, grey.b, grey.a);
		SDL_RenderFillRect(s.renderer, &rect);
		s.body.erase(s.body.begin());
	}
	SDL_RenderPresent(s.renderer); 
}

bool	checkColision(Cord& next, std::vector<Cord>& body)
{
	for (const Cord& cord : body)
	{
		if (cord.x == next.x && cord.y == next.y)
			return true;
	}
	return next.x >= BOARD_SIZE || next.x < 0 || next.y >= BOARD_SIZE || next.y < 0;
}

void	move(Snake& s)
{
	Cord	next = s.body.back();
	switch (s.dir)
	{
		case UP:    next.y--; break;
		case DOWN:  next.y++; break;
		case LEFT:  next.x--; break;
		case RIGHT: next.x++; break;
	}
	if (checkColision(next, s.body) == true)
	{
		std::cout << "You lose bro" << std::endl;
		s.isRunning = false;
		return ;
	}
	s.body.push_back(next);
	drawSnake(s);
	if (next.x == s.apple.x && next.y == s.apple.y)
	{
		s.apple = s.tiles[rand() % s.tiles.size()];
		drawApple(s);
	}
}

void	initSnake(SDL_Renderer* renderer, SDL_Window* window, Snake& s)
{
	srand(time(nullptr));

	s.renderer = renderer;
	s.window = window;

	s.dir = UP;

	SDL_SetWindowSize(s.window, BOARD_SIZE * (TILE_SIZE + SOFFSET) + SOFFSET, BOARD_SIZE * (TILE_SIZE + SOFFSET) + SOFFSET);

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
			s.tiles.push_back({x, y});
	}
	s.apple = {1 ,1};
	s.body.push_back({4, 4});
	SDL_SetRenderDrawColor(s.renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(s.renderer);
	clearBoard(s);
	drawSnake(s);
	drawApple(s);
}

void	snake::run(SDL_Renderer* renderer, SDL_Window* window)
{
	Snake	s;
	SDL_Event	e;
	auto	start = std::chrono::high_resolution_clock::now();

	initSnake(renderer, window, s);

	while (s.isRunning)
	{
		auto	now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = now - start;
		if (elapsed.count() >= 0.8)
		{
			move(s);
			start = now;
			s.buttonPressed = false;
		}
		while (SDL_PollEvent(&e))
		{
			
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				s.isRunning = false;
			if (e.type == SDL_KEYDOWN && !s.buttonPressed)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:    s.dir = UP;    break;
					case SDLK_DOWN:  s.dir = DOWN;  break;
					case SDLK_LEFT:  s.dir = LEFT;  break;
					case SDLK_RIGHT: s.dir = RIGHT; break;
				}
				s.buttonPressed = true;
			}
		}
	}
}