#include "../header/Tetris.hpp"
#include <iostream>

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
	setupBlock(t.blocks[YELLOW].cords, blockArray, startX, startY);
	t.blocks[YELLOW].colour = {235, 231, 9, 255};

	blockArray = {
		{0, 0, 0, 0},
		{1, 1, 1, 1}
	};
	setupBlock(t.blocks[LIGHT_BLUE].cords, blockArray, startX, startY);
	t.blocks[LIGHT_BLUE].colour = {9, 205, 235, 255};

	blockArray = {
		{1, 0, 0, 0},
		{1, 1, 1, 0}
	};
	setupBlock(t.blocks[DARK_BLUE].cords, blockArray, startX, startY);
	t.blocks[DARK_BLUE].colour = {0, 0, 143, 255};

	blockArray = {
		{1, 1, 0, 0},
		{0, 1, 1, 0}
	};
	setupBlock(t.blocks[RED].cords, blockArray, startX, startY);
	t.blocks[RED].colour = {171, 7, 7, 255};

	blockArray = {
		{0, 0, 1, 1},
		{0, 1, 1, 0}
	};
	setupBlock(t.blocks[GREEN].cords, blockArray, startX, startY);
	t.blocks[GREEN].colour = {12, 122, 21, 255};

	blockArray = {
		{0, 0, 0, 1},
		{0, 1, 1, 1}
	};
	setupBlock(t.blocks[ORANGE].cords, blockArray, startX, startY);
	t.blocks[ORANGE].colour = {212, 139, 21, 255};

	blockArray = {
		{0, 1, 0, 0},
		{1, 1, 1, 0}
	};
	setupBlock(t.blocks[PURPLE].cords, blockArray, startX, startY);
	t.blocks[PURPLE].colour = {119, 15, 171, 255};
}

void	drawBlock(const std::vector<Cord>& previous, const Block& current, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, grey.a);
	for (const Cord& cord : previous)
	{
		SDL_Rect	rect = { cord.x * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, cord.y * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE, TILE_SIZE };
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, current.colour.r, current.colour.g, current.colour.b, current.colour.a); 
	for (const Cord& cord : current.cords)
	{
		SDL_Rect	rect = { cord.x * (TILE_SIZE + BORDER_SIZE) + OFFSET, cord.y * (TILE_SIZE + BORDER_SIZE) + OFFSET, TILE_SIZE - OFFSET, TILE_SIZE - OFFSET };
		SDL_RenderFillRect(renderer, &rect);
	}
	SDL_RenderPresent(renderer);
}

void	drawTile(SDL_Rect tile, SDL_Color colour, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, &tile);
	SDL_RenderPresent(renderer);
}

void	drawNext(Tetris& t)
{
	Block	next = t.blocks[t.sequence[t.index]];

	SDL_SetRenderDrawColor(t.renderer, black.r, black.g, black.b, black.a);
	SDL_RenderFillRect(t.renderer, &t.nextRect);
	renderCenteredText(t.renderer, "Next:", t.nextRect);
	SDL_SetRenderDrawColor(t.renderer, next.colour.r, next.colour.g, next.colour.b, next.colour.a);
	for (const Cord& c : next.cords)
	{
		SDL_Rect	rect = {(c.x + 9) * (TILE_SIZE + BORDER_SIZE) + OFFSET, (c.y + 6) * (TILE_SIZE + BORDER_SIZE) + OFFSET, TILE_SIZE - OFFSET, TILE_SIZE - OFFSET};
		SDL_RenderFillRect(t.renderer, &rect);
	}
	SDL_RenderPresent(t.renderer);
}

void	initTetris(SDL_Renderer* renderer, SDL_Window* window, Tetris& t)
{
	srand(time(nullptr));
	t.window   = window;
	t.renderer = renderer;

	SDL_SetWindowSize(t.window, (BOARD_WIDTH + 8) * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, BOARD_HEIGHT * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE);
	setupBlocks(t);

	for (uint8_t i = 0; i < 7; i++)
		t.sequence.push_back(i);
	std::random_shuffle(t.sequence.begin(), t.sequence.end());

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

	t.textRect = {BOARD_WIDTH * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE + BORDER_SIZE, 8 * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, 256};
	drawTile(t.textRect, black, t.renderer);
	std::string	str = "Score: " + std::to_string(t.score) + " Level: " + std::to_string(t.level);
	renderCenteredText(t.renderer, str, t.textRect);

	
	t.current = t.blocks[t.sequence[t.index]];
	t.index++;
	drawBlock(t.current.cords, t.current, t.renderer);

	t.nextRect = {BOARD_WIDTH * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, 4 * (TILE_SIZE + BORDER_SIZE), 8 * (TILE_SIZE + BORDER_SIZE), 4 * (TILE_SIZE + BORDER_SIZE)};
	drawNext(t);
	SDL_RenderPresent(renderer);
}

void	handleTetris(Tetris& t, int y)
{
	for (; y > 0; y--)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			drawTile({ x * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, y * (TILE_SIZE + BORDER_SIZE) + BORDER_SIZE, TILE_SIZE, TILE_SIZE}, grey, t.renderer);
			t.board[y][x].exists = false;
			if (t.board[y - 1][x].exists == true)
			{
				drawTile({ x * (TILE_SIZE + BORDER_SIZE) + OFFSET, y * (TILE_SIZE + BORDER_SIZE) + OFFSET, TILE_SIZE - OFFSET, TILE_SIZE - OFFSET }, t.board[y - 1][x].colour, t.renderer);
				t.board[y][x] = t.board[y - 1][x];
			}
		}
	}
	SDL_RenderPresent(t.renderer);
}

void	updateBoard(Tetris& t)
{
	for (const Cord& cord : t.current.cords)
	{
		t.board[cord.y][cord.x] = {true, t.current.colour};
	}
	int	lines = 0;
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		// std::cout << "y: " << cord.y << " temp: " << temp << std::endl;
		int x = 0;
		for (; x < BOARD_WIDTH; x++)
		{
			if (t.board[y][x].exists == false) break;
		}
		if (x == BOARD_WIDTH)
		{
			handleTetris(t, y);
			lines++;
		}
	}
	t.score += (100 * lines * 1.5);
	if (t.score >= 1000 * t.level)
		t.level++;
	drawTile(t.textRect, black, t.renderer);
	std::string	str = "Score: " + std::to_string(t.score) + " Level: " + std::to_string(t.level);
	renderCenteredText(t.renderer, str, t.textRect);
}

bool	checkColision(Tetris& t)
{
	for (const Cord& cord : t.current.cords)
	{

		if (cord.y >= BOARD_HEIGHT - 1)                 return updateBoard(t), true;
		if (t.board[cord.y + 1][cord.x].exists == true) return updateBoard(t), true;
	}
	return false;
}

void	rotateBlock(Block& block)
{
	Cord	pivot = block.cords[1];

	for (Cord& c : block.cords)
	{
		int	relX = c.x - pivot.x;
		int	relY = c.y - pivot.y;

		c.x = pivot.x + -relY;
		c.y = pivot.y + relX;
	}
	for (Cord& c : block.cords)
	{
		if (c.x >= BOARD_WIDTH)
		{
			for (Cord& c : block.cords) { c.x--; };
		}
		if (c.x <= 0)
		{
			for (Cord& c : block.cords) { c.x++; };
		}
	}
}

void	holdBlock(Tetris& t)
{
	if (t.isHolding == true)
		return ;
	t.hold = t.current;
}

void	moveBlock(Direction dir, Tetris& t)
{
	Block	next = t.current;
	switch (dir)
	{
		case UP:    rotateBlock(next);                           break;
		case DOWN:  for (Cord& cord : next.cords) { cord.y++; }; break;
		case LEFT:
			for (Cord& c : next.cords)
			{
				c.x--;
				if (c.x < 0 || t.board[c.y][c.x].exists == true) return ;
			}; break;
		case RIGHT:
		for (Cord& c : next.cords)
		{
			c.x++;
			if (c.x >= BOARD_WIDTH || t.board[c.y][c.x].exists == true) return ;
		}; break;
	}
	drawBlock(t.current.cords, next, t.renderer);
	t.current = next;
	if (checkColision(t) == true)
	{
		t.current = t.blocks[t.sequence[t.index]];
		drawBlock(t.current.cords, t.current, t.renderer);
		t.index++;
		if (t.index == 7)
		{
			std::random_shuffle(t.sequence.begin(), t.sequence.end());
			t.index = 0;
		}
		drawNext(t);
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
		if (elapsed.count() >= (0.8 - 0.05 * t.level))
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
					case SDLK_w:     holdBlock(t);        break;
					case SDLK_UP:    moveBlock(UP,    t); break;
					case SDLK_DOWN:  moveBlock(DOWN,  t); break;
					case SDLK_LEFT:  moveBlock(LEFT,  t); break;
					case SDLK_RIGHT: moveBlock(RIGHT, t); break;
				}
			}
		}
	}
}