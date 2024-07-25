

#include <iostream>

#include "Game.h"

int main(int argc, char **argv)
{

	try
	{
		Game game;
		printf("loading");
		bool success = game.Initialize();
		printf("done loading");
		if (success)
		{
			game.RunLoop();
		}
		game.Shutdown();
	}
	catch (std::runtime_error e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}