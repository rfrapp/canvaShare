
#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Control.h"
#include "Button.h"
#include "TextBox.h"

class Canvas
{
private:
	// The width and height of the canvas
	int w, h;

	SDL_Renderer *renderer;
	std::vector< Control* > controls;

	// The foreground color for drawing
	Uint32 foreground;

	// The background color for filling 
	// shapes 
	Uint32 background; 

	Font *font;

public:
	Canvas(SDL_Renderer *r, int width, int height) 
	      : w(width), h(height), renderer(r),
	        foreground(0x00000000), background(0xFFFFFFFF)
	{
	}

	~Canvas()
	{
		// Clear the memory which was allocated for
		// the controls
		for (int i = 0; i < controls.size(); i++)
			delete controls[i];

		// Clear the memory allocated for the font
		font->destroy();
	}

	// Creates the Control objects needed for the
	// canvas and adds them to controls
	void init_controls();

	// Draws controls and CanvasObjects to the 
	// screen
	void draw();
	void handle_input(SDL_Event *e);

	// Loads fonts, images, etc.
	bool load_media();

	// Setter for the renderer property 
	void set_renderer(SDL_Renderer *r) { renderer = r; }
};

#endif
