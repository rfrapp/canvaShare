
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
		else if (canvas_items[i].get_type() == "triangle" && canvas_items[i].points.size() > 1)
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
		else if (canvas_items[i].get_type() == "circle" && canvas_items[i].points.size() > 1)
		{
			filledCircleRGBA(renderer, 
                   canvas_items[i].points[0].x + (canvas_items[i].points[1].x - 
                   	                              canvas_items[i].points[0].x) / 2,
                   canvas_items[i].points[0].y + (canvas_items[i].points[1].y - canvas_items[i].points[0].y) / 2, 
                   (canvas_items[i].points[1].x - canvas_items[i].points[0].x) / 2,
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

	if (controls[id]->has_focus() && !tools[id]->is_active())
		controls[id]->set_focus(false);
}

void Canvas::set_cursor(std::string c)
{
	cursor = c; 
}

void Canvas::init_controls()
{
	int start_x = 10;
	Button *paintbutton = new Button(PAINT_BRUSH_ID, this, renderer, 30, 30, start_x, 5, "", 
		        "images/icons.png", 0, 0, 30, 30);
	start_x += 35;

	Button *rectbutton = new Button(RECTANGLE_ID, this, renderer, 30, 30, start_x, 5, "", 
		        "images/icons.png", 154, 0, 30, 30);
	start_x += 35;

	Button *trianglebutton = new Button(TRIANGLE_ID, this, renderer, 30, 30, start_x, 5, "", 
		        "images/icons.png", 185, 0, 30, 30);
	start_x += 35;

	Button *circlebutton = new Button(CIRCLE_ID, this, renderer, 30, 30, start_x, 5, "",
		        "images/icons.png", 213, 0, 30, 30);
	start_x += 35;

	Button *undobutton = new Button(UNDO_ID, this, renderer, 30, 28, start_x, 5, "",
		        "images/icons.png", 60, 0, 28, 30);
	start_x += 35;

	Button *redobutton = new Button(REDO_ID, this, renderer, 30, 25, start_x, 5, "",
		        "images/icons.png", 88, 0, 25, 30);
	start_x += 35;

	Button *screenshotbutton = new Button(SCREENSHOT_ID, this, renderer, 30, 26, start_x, 5, "",
		        "images/icons.png", 308, 0, 26, 30);
	start_x += 35;

	PaintBrushTool *ptool      = new PaintBrushTool(this, draw_bounds);
	RectangleTool *recttool    = new RectangleTool(this, draw_bounds);
	TriangleTool *tritool      = new TriangleTool(this, draw_bounds);
	CircleTool *circletool     = new CircleTool(this, draw_bounds);
	UndoTool *undotool         = new UndoTool(this, draw_bounds);
	RedoTool *redotool         = new RedoTool(this, draw_bounds);
	ScreenshotTool *screentool = new ScreenshotTool(this, draw_bounds);

	// add paint brush tool
	tools.push_back(ptool);
	controls.push_back(paintbutton);

	// add rectangle
	tools.push_back(recttool);
	controls.push_back(rectbutton);

	// add triangle tool 
	tools.push_back(tritool);
	controls.push_back(trianglebutton);

	// add circle tool
	tools.push_back(circletool);
	controls.push_back(circlebutton);

	// add undo tool
	tools.push_back(undotool);
	controls.push_back(undobutton);

	// add redo tool
	tools.push_back(redotool);
	controls.push_back(redobutton);

	// add screenshot tool
	tools.push_back(screentool);
	controls.push_back(screenshotbutton);
}
