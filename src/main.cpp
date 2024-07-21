#include <iostream>

#include "Game.h"

int main(int argc, char** argv)
{

	try 
	{
		Game game;
		bool success = game.Initialize();
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