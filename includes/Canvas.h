
#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Control.h"
#include "Button.h"

class Canvas
{
private:
	int w, h;

	SDL_Renderer *renderer;
	std::vector< Control* > controls;

	Uint32 foreground;
	Uint32 background; 

public:
	Canvas(SDL_Renderer *r, int width, int height) 
	      : w(width), h(height), renderer(r),
	        foreground(0x000000), background(0xFFFFFF)
	{
	}

	~Canvas()
	{
		for (int i = 0; i < controls.size(); i++)
			delete controls[i];
	}

	void init_controls();

	void draw();
	void handle_input(SDL_Event *e) {  }

	void set_renderer(SDL_Renderer *r) { renderer = r; }
};

#endif
