
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

void Canvas::init_controls()
{
	Button *b = new Button(renderer, 30, 30, 100, 100, "", 
		        "images/SVG-icons.png", 4, 6, 30, 30);

	controls.push_back(b);
}
