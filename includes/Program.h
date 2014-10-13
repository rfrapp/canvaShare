
#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Canvas.h"
#include "LoginMenu.h"
#include "Network.h"

class Program
{
private:
	int screen_width, screen_height;
	bool quit;

	SDL_Window   *window;
	SDL_Renderer *renderer;
    SDL_Event    e;

    LoginMenu login_menu;
    Canvas canvas;

    bool is_online;
    Network * net;
    
public:
	Program(int w, int h);

	~Program() { cleanup(); }

	// Calls init() and load_media()
	bool initialize_program();

	// Initializes SDL
	bool init();
	
	// loads all images, fonts, etc
	// needed for the program
	bool load_media();

	// The main loop of the program
	int  execute();

	// Quits SDL and all of the subsystems
	// used as well as deallocates memory
	void cleanup();
};

#endif 
