#include "../header/Sequence.hpp"

int	main()
{

	try
	{
		Sequence	Sequence(4);
		Sequence.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n'
			<< SDL_GetError() << std::endl;
		SDL_Quit();
	}
	std::cout << "Until next time!\n";
}