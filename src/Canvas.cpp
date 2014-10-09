
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

	if (current_tool_index != -1)
		tools[current_tool_index]->draw(renderer);

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
		else if (canvas_items[i].get_type() == "rect" && canvas_items[i].points.size() > 1)
		{
			SDL_Rect r = {canvas_items[i].points[0].x, 
			              canvas_items[i].points[0].y,
			              canvas_items[i].points[1].x - canvas_items[i].points[0].x,
			              canvas_items[i].points[1].y - canvas_items[i].points[0].y};

			SDL_SetRenderDrawColor(renderer, canvas_items[i].get_background_r(),
				                   canvas_items[i].get_background_g(),
				                   canvas_items[i].get_background_b(),
				                   canvas_items[i].get_background_a());
			SDL_RenderFillRect(renderer, &r);
		}
		else if (canvas_items[i].get_type() == "triangle")
		{

			filledTrigonRGBA(renderer, (canvas_items[i].points[1].x + canvas_items[i].points[0].x) / 2, 
				      (canvas_items[i].points[1].y + canvas_items[i].points[0].y) / 2, 
				      canvas_items[i].points[1].x, canvas_items[i].points[1].y, 
                      canvas_items[i].points[0].x, canvas_items[i].points[1].y, 
                      canvas_items[i].get_background_r(), 
                      canvas_items[i].get_background_g(), 
                      canvas_items[i].get_background_b(), 
                      canvas_items[i].get_background_a());
		}
	}
}

void Canvas::handle_input(SDL_Event *e)
{
	// Checks if you clicked a button
	for (int i = 0; i < controls.size(); i++)
		controls[i]->handle_input(e);

	if (current_tool_index != -1)
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
	Button *b2 = new Button(RECTANGLE_ID, this, renderer, 30, 30, 45, 5, "", 
		        "images/icons.png", 154, 0, 30, 30);
	Button *b3 = new Button(TRIANGLE_ID, this, renderer, 30, 30, 80, 5, "", 
		        "images/icons.png", 185, 0, 30, 30);

	PaintBrushTool *p = new PaintBrushTool(this, draw_bounds);
	RectangleTool *r = new RectangleTool(this, draw_bounds);
	TriangleTool *t = new TriangleTool(this, draw_bounds);

	tools.push_back(p);
	controls.push_back(b);

	tools.push_back(r);
	controls.push_back(b2);

	tools.push_back(t);
	controls.push_back(b3);
}
