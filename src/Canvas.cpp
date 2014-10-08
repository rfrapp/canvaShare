
#include "Canvas.h"
#include "Animation.h"
#include "PaintBrushTool.h"
#include "Tool.h"

void Canvas::draw()
{
	SDL_Rect r = {draw_bounds.x, draw_bounds.y,
				  draw_bounds.w, draw_bounds.h};

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &r);

	// Draws all the controls on the canvas
	for (int i = 0; i < controls.size(); i++)
		controls[i]->draw();

	for (int i = 0; i < tools.size(); i++)
		tools[i]->draw(renderer);

	for (int i = 0; i < canvas_items.size(); i++)
	{
		if (canvas_items[i].get_type() == "paint")
		{
			// std::cout << canvas_items[i].get_brush_radius() << ' '
					  // << canvas_items[i].get_foreground_r() << ' '
					  // << canvas_items[i].get_foreground_g() << ' '
					  // << canvas_items[i].get_foreground_b() << ' '
					  // << canvas_items[i].get_foreground_a() << ' '
					  // << std::endl;

			for (int j = 0; j < canvas_items[i].points.size(); j++)
			{
				filledCircleRGBA(renderer, canvas_items[i].points[j].x, 
					       canvas_items[i].points[j].y, 
					       canvas_items[i].get_brush_radius(), 
					       canvas_items[i].get_foreground_r(), 
					       canvas_items[i].get_foreground_g(), 
					       canvas_items[i].get_foreground_b(), 
					       canvas_items[i].get_foreground_a());
			}
		}
	}
}

void Canvas::handle_input(SDL_Event *e)
{
	// Checks if you clicked a button
	for (int i = 0; i < controls.size(); i++)
		controls[i]->handle_input(e);

	tools[current_tool_index]->handle_input(e);
}

bool Canvas::load_media()
{
	font = new Font("fonts/cour.ttf", 24);

	return true;
}

void Canvas::get_notification(std::string event, int id)
{
	std::cout << "id: " << id << std::endl;
	for (int i = 0; i < controls.size(); i++)
	{
		if (i != id)
		{
			if (controls[i]->has_focus())
				controls[i]->set_focus(false);
		}
	}

	current_tool_index = id;
	tools[id]->toggle_activate();
}

void Canvas::set_cursor(std::string c)
{
	cursor = c; 
}

void Canvas::init_controls()
{
	Button *b = new Button(PAINT_BRUSH_ID, this, renderer, 30, 30, 10, 5, "", 
		        "images/icons.png", 0, 0, 30, 30);
	PaintBrushTool *p = new PaintBrushTool(this, draw_bounds);

	tools.push_back(p);
	controls.push_back(b);
}
