
#include "Canvas.h"

void Canvas::draw()
{
	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->draw();
	}
}

void Canvas::init_controls()
{
	Button *b = new Button(renderer, 34, 36, 100, 100, "", 
		        "images/SVG-icons.png", 0, 0, 34, 36);

	controls.push_back(b);
}
