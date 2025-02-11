#include "../header/Simple.hpp"

static SDL_Color	red   = {200, 100,  50, 255};
static SDL_Color	grey  = {128, 128, 128, 255};
static SDL_Color	blue  = {50,  100, 200, 255};
static SDL_Color	green = {50,  200, 100, 255};
static SDL_Color	black = {40, 40, 40, 255};

Simple::Simple()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw InitFailed();
	if (IMG_Init(IMG_INIT_PNG) == 0)
		throw InitFailed();
	_window = SDL_CreateWindow("simple", 640, 128, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
		throw InitFailed();
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (_renderer == nullptr)
	{
		SDL_DestroyWindow(_window);
		throw InitFailed();
	}
	_button = {	SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT - BUTTON_HEIGHT * 2, 
				BUTTON_WIDTH, BUTTON_HEIGHT};
	if (loadFontBM(_renderer) == false)
		throw InitFailed();
}

Simple::~Simple()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_DestroyTexture(fontTexture);
	IMG_Quit();
	SDL_Quit(); // always call this function
}

void	Simple::drawMenu()
{
	SDL_SetRenderDrawColor(_renderer, black.r, black.g, black.b, black.a);
	SDL_RenderClear(_renderer);

	SDL_SetRenderDrawColor(_renderer, grey.r, grey.g, grey.b, grey.a);
	SDL_RenderFillRect(_renderer, &_button);
	SDL_RenderPresent(_renderer);

	renderText(_renderer, "Simple Games", SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, BUTTON_HEIGHT);
	renderCenteredText(_renderer, "Snake", _button);
}

bool	Simple::isButtonPressed()
{
	return	_e.button.x >= _button.x && _e.button.x <= _button.x + BUTTON_WIDTH
			&& _e.button.y >= _button.y && _e.button.y <= _button.y + BUTTON_HEIGHT;
}

void	Simple::startGame()
{
	try
	{
		Snake	Snake(_renderer, _window);
		Snake.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	SDL_SetWindowSize(_window, SCREEN_WIDTH, SCREEN_HEIGHT);
	drawMenu();
}

void	Simple::run()
{
	drawMenu();
	while (_isRunning)
	{
		while (SDL_PollEvent(&_e))
		{
			
			if (_e.type == SDL_QUIT || (_e.type == SDL_KEYDOWN && _e.key.keysym.sym == SDLK_ESCAPE))
				_isRunning = false;
			if (_e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (_e.button.button == SDL_BUTTON_LEFT && isButtonPressed() == true)
				{
					startGame();
				}
			}
		}
	}
}