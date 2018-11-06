
/**
* @Author	Joseph Radu
* @date		14th February October 2017
* @brief	Creation of window, accessing application class for inputs and updates.
*
*/

#include <iostream>
#include <SDL/SDL.h>
#include "application.h"

int main(int argc, char* argv[])
{
	// Check that we were provided with the correct number of commanline arguments
	if (argc != 2) {
		std::cerr << "Usage GameEngine.exe GameDefinitionFile.xml" << std::endl;
		system("PAUSE");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Use the commandline argument which specifies the game definition file to load from
	application.Init(argv[1]);

	SDL_Event event;
	bool bExit = false;
	while (!bExit) {
		application.Update();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { // Did we press the cross on the window?
				bExit = true;
				break; // Don't process any more events after the user tried to quit
			}

			application.Event(event); // Pass all events to application
		}
	}

	application.Quit();

	SDL_Quit();
	return 0;
}
