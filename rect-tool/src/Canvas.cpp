
#include "Canvas.h"

void Canvas::draw()
{
	// Draws all the controls on the canvas
	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->draw();
	}
}

void Canvas::handle_input(SDL_Event *e)
{
	// Checks if you clicked a button
	for (int i = 0; i < controls.size(); i++)
		controls[i]->handle_input(e);
}

bool Canvas::load_media()
{
	font = new Font("fonts/cour.ttf", 24);

	return true;
}

void Canvas::init_controls()
{
	// Button *b = new Button(renderer, 30, 30, 100, 100, "", 
	// 	        "images/SVG-icons.png", 4, 6, 30, 30);
	TextBox *t = new TextBox(renderer, font, 200, h - 20, 10, 10,
		                                         true, false, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
		                                         0xBB, 0xBB, 0xBB);

	controls.push_back(t);
}