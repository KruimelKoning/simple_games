#include "../header/Sequence.hpp"

using namespace sequence;

static SDL_Color	red   = {200, 100,  50, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {50,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};

void	clearBoard(Sequencee& s)
{
	SDL_SetRenderDrawColor(s.renderer, grey.r, grey.g, grey.b, grey.a);
	for (SDL_Rect& tile : s.tiles)
		SDL_RenderFillRect(s.renderer, &tile);
	SDL_RenderPresent(s.renderer);
}

void	drawTile(const SDL_Color& colour, const SDL_Rect& tile, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, &tile);
	SDL_RenderPresent(renderer);
}

void	showSequence(Sequencee& s)
{
	SDL_SetRenderDrawColor(s.renderer, grey.r, grey.g, grey.b, grey.a);
	SDL_RenderFillRect(s.renderer, &s.textRect);
	renderCenteredText(s.renderer, "Level: " + std::to_string(s.level), s.textRect);
	SDL_RenderPresent(s.renderer);

	s.sequence.push_back(s.tiles[rand() % 9]);
	for (SDL_Rect& tile : s.sequence)
	{
		drawTile(s.colors[s.level - 1], tile, s.renderer);
		SDL_Delay(800);
		drawTile(grey, tile, s.renderer);
		SDL_Delay(200);
	}
	s.isWaiting = true;
}

bool	makeMove(int& i, SDL_Point mouseInput, Sequencee& s)
{
	// std::cout << "mouse x: " << mouseInput.x << " mouse y: " << mouseInput.y << std::endl;
	// std::cout << "sequence[" << i << "] x: " << _tiles[i].x << " sequence[" << i << "] y: "  << _tiles[i].y << std::endl;
	if (SDL_PointInRect(&mouseInput, &s.sequence[i]) == false)
		return std::cout << "YOU LOSE!\n", false;

	drawTile(s.colors[s.level - 1], s.sequence[i], s.renderer);
	SDL_Delay(400);
	drawTile(grey, s.sequence[i], s.renderer);
	i++;
	if (i == s.level)
	{
		SDL_Delay(800);
		s.level++;
		s.isWaiting = false;
		i = 0;
	}
	return true;
}

void	initSequence(SDL_Renderer* renderer, SDL_Window* window, Sequencee& s)
{
	s.renderer = renderer;
	s.window   = window;

	SDL_SetWindowSize(s.window, (BOARD_SIZE + 2) * (TILE_SIZE + TILE_OFFSET * 2), BOARD_SIZE * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET);

	s.tile = {0, 0, TILE_SIZE - BORDER_SIZE, TILE_SIZE - BORDER_SIZE};
	s.textRect = {TILE_SIZE * (BOARD_SIZE + 1) - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE, TILE_SIZE};

	for (int y = 0; y < BOARD_SIZE; y++)
	{
		for (int x = 0; x < BOARD_SIZE; x++)
			s.tiles.push_back({x * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET, y * (TILE_SIZE + TILE_OFFSET) + TILE_OFFSET, TILE_SIZE, TILE_SIZE});
	}
	int i = 0;
	for (; i < 3; i++)
		s.colors[i] = {50, (uint8_t)(200 - (i * 40)), 50, 255};
	for (; i < 6; i++)
		s.colors[i] = {50, 100, (uint8_t)(200 - (i * 20)), 255};
	for (; i < 9; i++)
		s.colors[i] = {(uint8_t)(200 - (i * 40)), 50, 50, 255};
	srand(time(nullptr));

	SDL_SetRenderDrawColor(s.renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(s.renderer);
	SDL_RenderPresent(s.renderer);
	clearBoard(s);
}

void	sequence::run(SDL_Renderer* renderer, SDL_Window* window)
{
	SDL_Event	e;
	Sequencee	s;

	initSequence(renderer, window, s);

	int	i = 0;
	while (s.isRunning && s.level <= BOARD_SIZE * BOARD_SIZE)
	{
		if (s.isWaiting == false)
			showSequence(s);
		while (SDL_PollEvent(&e))
		{
			
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				s.isRunning = false;
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					if (makeMove(i, {e.button.x, e.button.y}, s) == false)
						s.isRunning = false;
				}
			}
		}
	}
}