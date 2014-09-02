
#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class Program
{
private:
	int screen_width, screen_height;
	bool quit;

	SDL_Window   *window;
	SDL_Renderer *renderer;
    SDL_Event e;

public:
	Program(int w, int h) : screen_width(w), screen_height(h),
	                        window(NULL), renderer(NULL), quit(false)
	{ 
	}

	~Program() { cleanup(); }

	bool initialize_program();
	bool init();
	bool load_media();
	int  execute();
	void cleanup();
};

#endif 
