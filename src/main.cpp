#include "../header/Simple.hpp"

int	main()
{
	try
	{
		Simple	Simple;
		Simple.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'
			<< SDL_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
	}
	std::cout << "Until next time!\n";
}