
#include "Canvas.h"
#include "Animation.h"
#include "PaintBrushTool.h"

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

void Canvas::get_notification(std::string event, int id)
{
	for (int i = 0; i < controls.size(); i++)
	{
		if (controls[i]->has_focus())
			controls[i]->set_focus(false);
	}

	controls[id]->set_focus(true);
}

void Canvas::set_cursor(std::string c)
{
	cursor = c; 
}

void Canvas::init_controls()
{
	Button *b = new Button(0, this, renderer, 30, 30, 0, 0, "", 
		        "images/icons.png", 0, 0, 30, 30);
	PaintBrushTool p(this, "circle");

	controls.push_back(b);
}
