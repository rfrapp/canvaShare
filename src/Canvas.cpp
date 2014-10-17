
#include "Canvas.h"
#include "Animation.h"
#include "PaintBrushTool.h"
#include "Tool.h"
#include "Program.h"
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>

void Canvas::draw()
{
	SDL_Rect r = {draw_bounds.x, draw_bounds.y,
				  draw_bounds.w, draw_bounds.h};

	pages[current_page].set_as_target(renderer);

	// Draw the draw area boundaries 
	SDL_Rect screenrect = {0, 0, w, h};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &screenrect);

	// Draw canvas items 
	for (int i = 0; i < canvas_items.size(); i++)
	{
		if (canvas_items[i].get_page() == current_page)
		{
			if (canvas_items[i].get_type() == "paint")
			{
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

				filledTrigonRGBA(renderer, canvas_items[i].points[1].x - (canvas_items[i].points[1].x - canvas_items[i].points[0].x) / 2, 
				          canvas_items[i].points[0].y, 
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
			else if (canvas_items[i].get_type() == "erase")
			{
				for (int j = 0; j < canvas_items[i].points.size(); j++)
				{
					filledCircleRGBA(renderer, canvas_items[i].points[j].x, 
						       canvas_items[i].points[j].y, 
						       canvas_items[i].get_brush_radius(), 
						       255, 255, 255, 255);
				}
			}
			else if (canvas_items[i].get_type() == "pencil")
			{
				for (int j = 0; j < canvas_items[i].points.size(); j++)
				{
					pixelRGBA(renderer, canvas_items[i].points[j].x, 
						       canvas_items[i].points[j].y, 
						       canvas_items[i].get_foreground_r(), 
						       canvas_items[i].get_foreground_g(), 
						       canvas_items[i].get_foreground_b(), 
						       canvas_items[i].get_foreground_a());
				}
			}
			else if (canvas_items[i].get_type() == "textbox")
			{
				// SDL_SetRenderDrawColor(renderer, 0xBB, 0xBB, 0xBB, 0xFF);
				// SDL_Rect rectangle = canvas_items[i].get_bounding_rect().getSDL_Rect();

				// SDL_RenderDrawRect(renderer, &rectangle);
			}
		}
	}

	for (int i = 0; i < drawn_textboxes.size(); i++)
	{
		if (drawn_textboxes[i].get_page() == current_page)
			drawn_textboxes[i].draw();
	}

	// Set the render target back to the default (the window)
	SDL_SetRenderTarget(renderer, NULL);

	// Draw the draw area boundaries 
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &r);
	
	// render the current page 
	SDL_Rect tmp_rect = draw_bounds.getSDL_Rect();
	tmp_rect.w -= 2;
	tmp_rect.h -= 2;

	pages[current_page].render(renderer, draw_bounds.x + 1, draw_bounds.y + 1, &tmp_rect);

	// Draw a white rectangle around the screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(renderer, &screenrect);

	// Draws all the controls on the canvas
	for (int i = 0; i < controls.size(); i++)
		controls[i]->draw();

	if (current_tool_index != -1)
		tools[current_tool_index]->draw(renderer);

	// Draw a rectangle of the foreground color
	SDL_Rect fg_rect = foreground_rect.getSDL_Rect();
	SDL_Rect fg_o_rect = {fg_rect.x - 1, fg_rect.y - 1, fg_rect.w + 2, 
	                      fg_rect.h + 2};
	
	if (!background_selected)
		SDL_SetRenderDrawColor(renderer, 255, 255, 96, 255);
	else
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	SDL_RenderFillRect(renderer, &fg_o_rect);

	SDL_SetRenderDrawColor(renderer, fg_r, fg_g, fg_b, 255);
	SDL_RenderFillRect(renderer, &fg_rect);

	// Draw a rectangle of the background color
	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255);
	SDL_Rect bg_rect = background_rect.getSDL_Rect();
	SDL_Rect bg_o_rect = {bg_rect.x - 1, bg_rect.y - 1, bg_rect.w + 2, 
	                      bg_rect.h + 2};
	
	if (background_selected)
		SDL_SetRenderDrawColor(renderer, 255, 255, 96, 255);
	else
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
	SDL_RenderFillRect(renderer, &bg_o_rect);

	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255);
	SDL_RenderFillRect(renderer, &bg_rect);

	// draw rects for colors
	for (int i = 0; i < color_rects.size(); i++)
	{
		SDL_Rect tmp_c_rect = color_rects[i].getSDL_Rect();
		int r = 0, g = 0, b = 0;

		SDL_SetRenderDrawColor(renderer, color_rects[i].r, color_rects[i].g, color_rects[i].b, 255);
		SDL_RenderFillRect(renderer, &tmp_c_rect);
	}

	// Draw the "Page x of y" label
	page_surface->render(renderer, 10, h - 35);

}

void Canvas::handle_input(SDL_Event *e)
{
	// Checks if you clicked a button
	for (int i = 0; i < controls.size(); i++)
		controls[i]->handle_input(e);

	if (current_tool_index != -1)
		tools[current_tool_index]->handle_input(e);

	for (int i = 0; i < drawn_textboxes.size(); i++)
	{
		if (drawn_textboxes[i].get_page() == current_page)
			drawn_textboxes[i].handle_input(e);
	}

	if (current_tool_index > -1)
	{
		if (!tools[current_tool_index]->is_active() && controls[current_tool_index]->has_focus())
		{
			controls[current_tool_index]->set_focus(false);
			SDL_ShowCursor(SDL_ENABLE);
			//current_tool_index = -1;
		}
	}

	if (e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (background_rect.collide_point(x, y))
			background_selected = true;
		else if (foreground_rect.collide_point(x, y))
			background_selected = false;

		for (int i = 0; i < color_rects.size(); i++)
		{
			if (color_rects[i].collide_point(x, y))
			{
				if (background_selected)
				{
					bg_r = color_rects[i].r;
					bg_g = color_rects[i].g;
					bg_b = color_rects[i].b;
				}
				else
				{
					fg_r = color_rects[i].r;
					fg_g = color_rects[i].g;
					fg_b = color_rects[i].b;
				}
			}
		}

		bool item_selected = false;

		for (int i = canvas_items.size() - 1; i >= 0; i--)
		{
			Rect b_rect = canvas_items[i].get_bounding_rect();
			
			//std::cout << b_rect << std::endl;
			
			if (b_rect.collide_point(x, y) && is_move_tool() && 
				canvas_items[i].get_type() != "erase" && 
				canvas_items[i].get_page() == current_page)
			{
				selected_item = i;
				item_selected = true;

				if (clicked_pos != NULL)
					delete clicked_pos;

				clicked_pos = new Point;
				clicked_pos->x = x;
				clicked_pos->y = y;
			}
		}
	}
}

bool Canvas::is_move_tool() const 
{
	bool move = true;

	for (int i = 0; i < tools.size(); i++)
	{
		if (tools[i]->is_active())
			move = false;
	}

	return move;
}

bool Canvas::load_media()
{
	font = new Font("fonts/cour.ttf", 24);

	return true;
}

void Canvas::get_notification(std::string event, int id)
{
	// std::cout << "id: " << id << std::endl;
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
	Button *paintbutton = new Button(PAINT_BRUSH_ID, this, renderer, font, 30, 30, start_x, 5, "Paintbrush tool", 
		        "images/icons.png", 0, 0, 30, 30);
	start_x += 35;

	Button *pencilbutton = new Button(PENCIL_ID, this, renderer, font, 27, 30, start_x, 5, "Pencil tool", 
		        "images/icons.png", 278, 0, 27, 30);
	start_x += 35;	

	Button *rectbutton = new Button(RECTANGLE_ID, this, renderer, font, 30, 30, start_x, 5, "Rectangle tool", 
		        "images/icons.png", 154, 0, 30, 30);
	start_x += 35;

	Button *trianglebutton = new Button(TRIANGLE_ID, this, renderer, font, 30, 30, start_x, 5, "Triangle tool", 
		        "images/icons.png", 185, 0, 30, 30);
	start_x += 35;

	Button *circlebutton = new Button(CIRCLE_ID, this, renderer, font, 30, 30, start_x, 5, "Circle tool",
		        "images/icons.png", 213, 0, 30, 30);
	start_x += 35;

	Button *undobutton = new Button(UNDO_ID, this, renderer, font, 30, 31, start_x, 5, "Undo",
		        "images/icons.png", 337, 0, 31, 30);
	start_x += 35;

	Button *redobutton = new Button(REDO_ID, this, renderer, font, 30, 31, start_x, 5, "Redo",
		        "images/icons.png", 371, 0, 31, 30);
	start_x += 35;

	Button *screenshotbutton = new Button(SCREENSHOT_ID, this, renderer, font, 30, 26, start_x, 5, "Export",
		        "images/icons.png", 308, 0, 26, 30);
	start_x += 35;

	Button *previous_page_button = new Button(PREVPAGE_ID, this, renderer, font, 30, 26, w / 2 - 35, h - 35, "Previous page",
		        "images/icons.png", 61, 0, 26, 30);

	Button *next_page_button = new Button(NEXTPAGE_ID, this, renderer, font, 30, 25, w / 2 + 35, h - 35, "Next page",
		        "images/icons.png", 87, 0, 25, 30);

	Button *eraserbutton = new Button(ERASE_ID, this, renderer, font, 26, 30, start_x, 5, "Eraser tool", 
		        "images/icons.png", 496, 0, 26, 30);
	start_x += 35;

	Button *textboxbutton = new Button(TEXT_ID, this, renderer, font, 22, 30, start_x, 5, "Textbox tool", 
		        "images/icons.png", 601, 0, 22, 30);
	start_x += 35;

	PaintBrushTool *ptool      = new PaintBrushTool(this, draw_bounds);
	PencilTool *penciltool     = new PencilTool(this, draw_bounds);
	RectangleTool *recttool    = new RectangleTool(this, draw_bounds);
	TriangleTool *tritool      = new TriangleTool(this, draw_bounds);
	CircleTool *circletool     = new CircleTool(this, draw_bounds);
	UndoTool *undotool         = new UndoTool(this, draw_bounds);
	RedoTool *redotool         = new RedoTool(this, draw_bounds);
	ScreenshotTool *screentool = new ScreenshotTool(this, draw_bounds);
	NextPageTool *nextpagetool = new NextPageTool(this, draw_bounds);
	PreviousPageTool *prevtool = new PreviousPageTool(this, draw_bounds);
	EraserTool *erasetool      = new EraserTool(this, draw_bounds);
	TextBoxTool *textboxtool   = new TextBoxTool(this, draw_bounds);

	// add paint brush tool
	tools.push_back(ptool);
	controls.push_back(paintbutton);

	// add pencil tool
	tools.push_back(penciltool);
	controls.push_back(pencilbutton);

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

	// add previous page tool
	controls.push_back(previous_page_button);
	tools.push_back(prevtool);

	// add next page tool
	controls.push_back(next_page_button);
	tools.push_back(nextpagetool);

	// add the eraser tool
	tools.push_back(erasetool);
	controls.push_back(eraserbutton);

	tools.push_back(textboxtool);
	controls.push_back(textboxbutton);

	// setup the rectangles for colors and the
	// foreground and background
	foreground_rect.x = w - 105;
	foreground_rect.y = 5;
	foreground_rect.w = 30;
	foreground_rect.h = 30;

	background_rect.x = w - 70;
	background_rect.y = 5;
	background_rect.w = 30;
	background_rect.h = 30;

	// Set up color rectangles 
	int start_y = 40;
	ColorRect black_rect(w - 35, start_y, 30, 30, 0, 0, 0);
	start_y += 35;

	ColorRect blue_rect(w - 35, start_y, 30, 30, 0, 0, 255);
	start_y += 35;

	ColorRect red_rect(w - 35, start_y, 30, 30, 255, 0, 0);
	start_y += 35;

	ColorRect green_rect(w - 35, start_y, 30, 30, 0, 255, 0);
	start_y += 35;

	ColorRect yellow_rect(w - 35, start_y, 30, 30, 255, 255, 96);	
	start_y += 35;

	color_rects.push_back(black_rect);
	color_rects.push_back(blue_rect);
	color_rects.push_back(red_rect);
	color_rects.push_back(green_rect);
	color_rects.push_back(yellow_rect);

}

void Canvas::add_page()
{
	Texture t;
	t.load_blank(renderer, draw_bounds.w, draw_bounds.h);
	pages.push_back(t);	

	if (pages.size() > 1)
		current_page++;

	std::cout << pages.size() << std::endl;

	if (page_surface != NULL)
		delete page_surface;

	page_surface = new TextSurface(font, 0, 0, 0, 0, false);
	std::stringstream stream;
	stream << "Page " << current_page + 1 << " of " << pages.size();
	page_surface->set_text(renderer, stream.str());
}


std::string Canvas::canvas_items_to_string()
{
	std::string str;
	//std::string stream;
	for (int i = 0; i < canvas_items.size(); i++)
	{
		str += (canvas_items[i].to_string());
	}
	//str = stream.c_str();

	return str;
}

void Canvas::receive_message(const std::string & str)
{
	std::stringstream stream;
	std::string line;
	std::vector< std::string > lines; 

	stream << str;

	while (getline(stream, line))
	{
		lines.push_back(line);
		// std::cout << "line: " << line << std::endl;
	}

	if (lines[0] == "modify pop")
	{
		Point p = {std::atoi(lines[1].c_str()), std::atoi(lines[2].c_str())};
		add_point_to_item(p, true, false);
	}
	else if (lines[0] == "modify")
	{
		Point p = {std::atoi(lines[1].c_str()), std::atoi(lines[2].c_str())};
		add_point_to_item(p, false, false);
	}
	else if (lines[0] == "add")
	{
		std::cout << "\n\nI AM IN ADD COMMAND\n\n" << std::endl;
		// collapse the lines to 1 string
		std::string newstr = "";

		for (int i = 1; i < lines.size(); i++)
			newstr += lines[i] + '\n';

		CanvasItem c = CanvasItem::string_to_item(newstr);
		std::cout << "New canvas item: " << std::endl;
		std::cout << c.to_string();
		add_canvas_item(c, false);
	}
	else if (lines[0] == "undo")
		undo_canvas_item(false);
	else if (lines[0] == "redo")
		redo_canvas_item(false);
	else if (lines[0] == "page right")
		page_right(false);
	else if (lines[0] == "page left")
		page_left(false);
	else if (lines[0] == "insert")
		modify_textbox(str, false);
	else if (lines[0] == "delete")
		modify_textbox(str, false);
	else if (lines[0] == "enter")
		modify_textbox(str, false);
}

void Canvas::add_canvas_item(const CanvasItem & i, bool send) 
{ 
	CanvasItem item = i;
	item.set_page(current_page);

	undone_items.clear();
	canvas_items.push_back(item);

	if (send)
		parent->send_message("add\n" + item.to_string());

	if (item.get_type() == "textbox")
	{
		TextBox t(renderer, font, 1, 1, item.points[0].x, 
			      item.points[0].y, true, true, 255, 255, 255,
			      fg_r, fg_g, fg_b);
		t.set_page(current_page);
		t.set_parent(this);
		t.set_send(true);
		drawn_textboxes.push_back(t);

		for (int i = drawn_textboxes.size() - 2; i >= 0; i--)
		{
			if (drawn_textboxes[i].has_focus())
				drawn_textboxes[i].set_focus(false);
		}
	}
}

void Canvas::resize_textbox(const int & _w, const int & _h, bool send)
{
	drawn_textboxes[drawn_textboxes.size() - 1].set_dimensions(_w, _h);
}

void Canvas::add_point_to_item(const Point & p, bool pop, bool send)
{
	// send over network here
	std::stringstream stream; 
	stream << "modify";

	if (pop)
	{
		stream << " pop"; 
		if (canvas_items[canvas_items.size() - 1].points.size() == 2)
			canvas_items[canvas_items.size() - 1].points.pop_back();
	}

	if (send)
	{
		stream << "\n";
		stream << p.x << '\n';
		stream << p.y << '\n';
		// std::cout << "message: " << std::endl;
		// std::cout << stream.str();
		parent->send_message(stream.str());
	}
	canvas_items[canvas_items.size() - 1].points.push_back(p);
	CanvasItem item = canvas_items[canvas_items.size() - 1];

	if (item.get_type() == "textbox")
	{
		int _w = item.points[1].x - item.points[0].x;
		int _h = item.points[1].y - item.points[0].y;
		resize_textbox(_w, _h, false);
	}

	// std::cout << "size of points: " << canvas_items[canvas_items.size() - 1].points.size() << std::endl;
}

void Canvas::modify_textbox(std::string str, bool send)
{
	std::stringstream stream;
	std::string line;
	std::vector< std::string > lines; 
	int modified_index = 0;

	stream << str;

	while (getline(stream, line))
	{
		lines.push_back(line);
		// std::cout << "line: " << line << std::endl;
	}

	for (int i = drawn_textboxes.size() - 1; i >= 0; i--)
	{
		if (drawn_textboxes[i].has_focus())
		{
			modified_index = i;
			break;
		}
	}

	if (send)
	{
		std::stringstream stream2;
		if (lines[0] == "insert")
		{
			stream2 << "insert"       << '\n'
				   << modified_index << '\n'
		           << lines[1]       << '\n'
		           << lines[2]       << '\n'
		           << lines[3]       << '\n';
		}
		else if (lines[0] == "delete")
		{
			stream2 << "delete"       << '\n'
				    << modified_index << '\n'
		            << lines[1]       << '\n'
		            << lines[2]       << '\n';
		}
		else if (lines[0] == "enter")
		{
			stream2 << "enter"        << '\n';
			stream2 << modified_index << '\n';
			stream2 << lines[1]       << '\n';
			stream2 << lines[2]       << '\n';
		}

		parent->send_message(stream2.str());
	}
	else
	{
		if (lines[0] == "insert")
		{
			int i = std::atoi(lines[1].c_str());
			int l = std::atoi(lines[2].c_str());
			int p = std::atoi(lines[3].c_str());
			char c = lines[4][0];

			drawn_textboxes[i].insert_char(l, p, c);
		}
		else if (lines[0] == "delete")
		{
			int i = std::atoi(lines[1].c_str());
			int l = std::atoi(lines[2].c_str());
			int p = std::atoi(lines[3].c_str());

			std::cout << "I AM DELETING STUFF" << std::endl;

			drawn_textboxes[i].delete_char(l, p);
		}
		else if (lines[0] == "enter")
		{
			int i = std::atoi(lines[1].c_str());
			int l = std::atoi(lines[2].c_str());
			int p = std::atoi(lines[3].c_str());

			drawn_textboxes[i].to_next_line(l, p);
		}
	}
}

/*
void Canvas::send_canvas()
{
	const char* c_items;
	std::string str = this->canvas_items_to_string();
	c_items = str.c_str();
	SDLNet_TCP_Send(server, c_items, strlen(c_items)+1);
}
*/

void Canvas::undo_canvas_item(bool send) 
{ 
	// TODO: Send message to network
	// Somethong like "undo" to to let
	// the other client know what happened 
	if (send)
		parent->send_message("undo");

	if (canvas_items.size() > 0)
	{
		CanvasItem item = canvas_items[canvas_items.size() - 1];
		canvas_items.pop_back();
		undone_items.push_back(item);

		if (item.get_type() == "textbox")
		{
			TextBox t = drawn_textboxes[drawn_textboxes.size() - 1];
			undone_textboxes.push_back(t);
			drawn_textboxes.pop_back();
		}

		if (item.get_page() != current_page)
			current_page = item.get_page();
	}
}

void Canvas::redo_canvas_item(bool send)
{
	// TODO: Send message to network
	// Somethong like "redo" to to let
	// the other client know what happened
	if (send)
		parent->send_message("redo");

	if (undone_items.size() > 0)
	{
		CanvasItem item = undone_items[undone_items.size() - 1];
		undone_items.pop_back();
		canvas_items.push_back(item);

		if (item.get_type() == "textbox")
		{
			TextBox t = undone_textboxes[undone_textboxes.size() - 1];
			drawn_textboxes.push_back(t);
			undone_textboxes.pop_back();
		}

		if (item.get_page() != current_page)
			current_page = item.get_page();
	}
}

void Canvas::page_left(bool send)
{
	if (current_page > 0)
	{
		if (send)
			parent->send_message("page left");
		
		current_page--;
		
		page_surface = new TextSurface(font, 0, 0, 0, 0, false);
		std::stringstream stream;
		stream << "Page " << current_page + 1 << " of " << pages.size();
		page_surface->set_text(renderer, stream.str());
	}
}

void Canvas::page_right(bool send)
{
	if (send)
		parent->send_message("page right");

	if (pages.size() - 1 == current_page)
	{
		add_page();
	}
	else 
	{
		current_page++;

		page_surface = new TextSurface(font, 0, 0, 0, 0, false);
		std::stringstream stream;
		stream << "Page " << current_page + 1 << " of " << pages.size();
		page_surface->set_text(renderer, stream.str());
	}
}


void Canvas::set_id(int i)
{
    is_ready = true;
    id = i;
}
