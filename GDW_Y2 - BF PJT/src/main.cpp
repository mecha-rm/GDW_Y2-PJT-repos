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
		- NOTE: this developer is no longer part of the program.
 
 * Game: CODENAME: ZERO
 * Engine: CHERRY ENGINE
 * Date: 08/22/2020
 * Description: the project for the Game Development Workshop game engine. The game code is stored in a seperate project.
 * References: 
	* Learn OpenGL (https://learnopengl.com/)
	* GLFW (https://www.glfw.org/)
	* CPPReference (https://en.cppreference.com/w/)
	* Cplusplus.com (http://www.cplusplus.com/)
	* Wavefront (Obj) and Material Template Library (Mtl) Loader:
		* https://en.wikipedia.org/wiki/Wavefront_.obj
			* https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library
			* https://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
			* http://paulbourke.net/dataformats/mtl/
*/

// Bonus Fruit - Cherry Engine
#include "cherry/Game.h"
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

	Logger::Init();

	// game object
	cherry::Game* game = new cherry::Game("GDW_Y2 - Bonus Fruit - Cherry Engine", 850, 850, false, true, nullptr, true);
	cherry::Game::FPS = 0;
	game->Run();
	delete game;

	Logger::Uninitialize();

	return 0;
}