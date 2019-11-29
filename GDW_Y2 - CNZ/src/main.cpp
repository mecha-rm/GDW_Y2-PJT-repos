/*
 * Team: BONUS FRUIT
 * Developers:
	- Kennedy Adams	(100632983)
	- Jonah Griffin	(100702748)
	- Nathan Tuck	(100708651)
	- Spencer Tester (100653129)
	- Stephane Gagnon (100694227)
	- Roderick "R.J." Montague (100701758)
	- Ryan Burton (100707511)
 
 * Game: CODENAME: ZERO
 * Engine: CHERRY ENGINE
 * Date: 11/28/2019
 * Description: the game development game. The engine files are in the 'cherry' folder, but there is a dedicated engine project.
 * References: 
	* NOTE: The engine has its own set of references
*/

#include "CNZ_Game.h"
#include <toolkit/Logging.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // checks for memory leaks once the program ends.
	long long allocPoint = 0;
	if (allocPoint)
	{
		_CrtSetBreakAlloc(allocPoint); // sets where you want to stop our program by assigning the allocation block index stopping point.
	}

	// stbi_image_free(data);
	// glfwSetWindowSize(window, 1000, 1000);

	Logger::Init();


	// cherry::Game* game = new cherry::Game(850, 850, false);
	cherry::Game* game = new cnz::CNZ_Game(850, 850, false); // creating a Codename Zero game.
	game->Run();
	delete game;

	Logger::Uninitialize();

	return 0;
}

