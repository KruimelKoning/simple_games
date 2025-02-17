#include "../header/Tetris.hpp"

static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	black = {40, 40, 40, 255};

void	setupBlock(std::vector<Cord>& block, std::vector<std::vector<bool>> blockArray, int startX, int startY)
{
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (blockArray[y][x] == true)
				block.push_back({startX + x, startY + y});
		}
	}
}

void	setupBlocks(Tetris& t)
{
	int	startX = (BOARD_WIDTH / 2) - 2;
	int	startY = 0;

	std::vector<std::vector<bool>>	blockArray = {
		{0, 1, 1, 0},
		{0, 1, 1, 0}
	};
	setupBlock(t.blocks[YELLOW], blockArray, startX, startY);
	t.colours[YELLOW] = {235, 231, 9, 255};

	blockArray = {
		{0, 0, 0, 0},
		{1, 1, 1, 1}
	};
	setupBlock(t.blocks[LIGHT_BLUE], blockArray, startX, startY);
	t.colours[LIGHT_BLUE] = {9, 205, 235, 255};

	blockArray = {
		{1, 0, 0, 0},
		{1, 1, 1, 0}
	};
	setupBlock(t.blocks[DARK_BLUE], blockArray, startX, startY);
	t.colours[DARK_BLUE] = {0, 0, 143, 255};

	blockArray = {
		{1, 1, 0, 0},
		{0, 1, 1, 0}
	};
	setupBlock(t.blocks[RED], blockArray, startX, startY);
	t.colours[RED] = {171, 7, 7, 255};

	blockArray = {
		{0, 0, 1, 1},
		{0, 1, 1, 0}
	};
	setupBlock(t.blocks[GREEN], blockArray, startX, startY);
	t.colours[GREEN] = {12, 122, 21, 255};

	blockArray = {
		{0, 0, 0, 1},
		{0, 1, 1, 1}
	};
	setupBlock(t.blocks[ORANGE], blockArray, startX, startY);
	t.colours[ORANGE] = {212, 139, 21, 255};

	blockArray = {
		{0, 1, 0, 0},
		{1, 1, 1, 0}
	};
	setupBlock(t.blocks[PURPLE], blockArray, startX, startY);
	t.colours[PURPLE] = {119, 15, 171, 255};
}

void	drawBlock(const std::vector<Cord>& previous, const std::vector<Cord>& current, SDL_Color colour, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, grey.a);
	for (const Cord& cord : previous)
	{
		SDL_Rect	rect = { cord.x * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, cord.y * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE, TILE_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	for (const Cord& cord : current)
	{
		SDL_Rect	rect = { cord.x * (TILE_SIZE + BORDER_SIZE) + OFFSET, cord.y * (TILE_SIZE + BORDER_SIZE) + OFFSET, TILE_SIZE - OFFSET, TILE_SIZE - OFFSET };
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_RenderPresent(renderer);
}

void	initTetris(SDL_Renderer* renderer, SDL_Window* window, Tetris& t)
{
	srand(time(nullptr));
	t.window   = window;
	t.renderer = renderer;

	SDL_SetWindowSize(t.window, BOARD_WIDTH * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, BOARD_HEIGHT * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE);
	setupBlocks(t);

	SDL_SetRenderDrawColor(t.renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(t.renderer);
	SDL_SetRenderDrawColor(t.renderer, grey.r, grey.g, grey.b, grey.a);
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			SDL_Rect	rect = { x * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, y * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderFillRect(t.renderer, &rect);
		}
	}

	int	random = rand() % 7;
	t.current = t.blocks[random];
	drawBlock(t.current, t.current, t.colours[random], t.renderer);
	SDL_RenderPresent(renderer);
}

void	updateBoard(Tetris& t)
{
	for (const Cord& cord : t.current)
	{
		t.board[cord.y][cord.x] = true;
	}
}

bool	checkColision(Tetris& t)
{
	for (const Cord& cord : t.current)
	{
		if (cord.y >= BOARD_HEIGHT - 1)          return updateBoard(t), true;
		if (t.board[cord.y + 1][cord.x] == true) return updateBoard(t), true;
	}
	return false;
}

void	moveBlock(Direction dir, Tetris& t)
{
	std::vector<Cord>	next = t.current;
	switch (dir)
	{
		case DOWN:  for (Cord& cord : next) { cord.y++; }; break;
		case LEFT:  for (Cord& cord : next) { cord.x--; }; break;
		case RIGHT: for (Cord& cord : next) { cord.x++; }; break;
	}
	drawBlock(t.current, next, t.colours[GREEN], t.renderer);
	t.current = next;
	if (checkColision(t) == true)
	{
		int	random = rand() % 7;
		t.current = t.blocks[random];
		drawBlock(t.current, t.current, t.colours[random], t.renderer);
	}
}

void	tetris::run(SDL_Renderer* renderer, SDL_Window* window)
{
	Tetris		t;
	SDL_Event	e;

	initTetris(renderer, window, t);

	auto	start = std::chrono::high_resolution_clock::now();

	while (t.isRunning)
	{
		auto	now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = now - start;
		if (elapsed.count() >= 0.8)
		{
			moveBlock(DOWN, t);
			start = now;
		}
		while (SDL_PollEvent(&e))
		{
			
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				t.isRunning = false;
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_DOWN:  moveBlock(DOWN,  t); break;
					case SDLK_LEFT:  moveBlock(LEFT,  t); break;
					case SDLK_RIGHT: moveBlock(RIGHT, t); break;
				}
			}
		}
	}
}