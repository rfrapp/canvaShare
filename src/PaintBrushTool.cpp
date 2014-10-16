
#include "Canvas.h"
#include "PaintBrushTool.h"
#include <cmath>

PaintBrushTool::PaintBrushTool(Canvas * p, Rect r)
			   : Tool(p, r), item(NULL), clicked(false)
{
}

void PaintBrushTool::handle_input(SDL_Event *e) 
{ 
	if (active)
	{
		int x, y;
   		SDL_GetMouseState(&x, &y);
	
		if (e->type == SDL_KEYUP)
		{
			if(e->key.keysym.sym == SDLK_EQUALS && parent->get_brush_radius() < 15)
	        {
	        	parent->increment_brush_radius();
	        }
	        else if(e->key.keysym.sym == SDLK_MINUS && parent->get_brush_radius() > 0)
	        {
	        	parent->decrement_brush_radius();
	        }
    	}
    	else if (e->type == SDL_MOUSEBUTTONDOWN && draw_bounds.collide_point(x, y))
    	{
			if (item != NULL) delete item; 

    		item = new CanvasItem("paint", 
							  parent->get_foreground_r(), parent->get_foreground_g(),
							  parent->get_foreground_b(), parent->get_foreground_a(),
			                  parent->get_background_r(), parent->get_background_g(),
			                  parent->get_background_b(), parent->get_background_a(), 
			                  parent->get_brush_radius());
    		
    		item->set_brush_radius(parent->get_brush_radius());
    		clicked = true;
    		Point p = {x, y};

    		item->points.push_back(p);
    		item->set_brush_radius(parent->get_brush_radius());
    		parent->add_canvas_item(*item);
    	}
    	else if (e->type == SDL_MOUSEMOTION && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
	    		Point p = {x, y};
    			parent->get_items()[parent->get_items().size() - 1].set_brush_radius(parent->get_brush_radius());
    			
    			// int x0 = item->points[item->points.size() - 1].x;
	    		// int x1 = x;
	    		// int y0 = item->points[item->points.size() - 1].y;
	    		// int y1 = y;

	    		// y = y0;
	    		// int dx = x1 - x0;

	    		// if (dx < 0)
	    		// {
	    		// 	int t = x0;
	    		// 	x0 = x1;
	    		// 	x1 = t;
	    		// 	dx = x1 - x0;
	    		// }

	    		// int dy = y1 - y0;
	    		// double error = 0.0;
	    		// double derror = abs((double)dy / dx);

	    		// // Bresenham's Line Algorithm
	    		// for (int i = x0; i <= x1; i++)
	    		// {
	    		// 	Point p = {i, y};
	    		// 	error += derror; 

	    		// 	if (error >= 0.5)
	    		// 	{
	    		// 		y++;
	    		// 		error -= 1.0;
	    		// 	}

	    		// 	parent->add_point_to_item(p);
	    		// }

    			parent->add_point_to_item(p, false);
    			// parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
    	else if (e->type == SDL_MOUSEBUTTONUP && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
	    		Point p = {x, y};

	    		clicked = false;
	    		parent->get_items()[parent->get_items().size() - 1].set_brush_radius(parent->get_brush_radius());
	    		parent->add_point_to_item(p, false);
	    		// parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
	}
}

void PaintBrushTool::draw(SDL_Renderer *r)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (active && draw_bounds.collide_point(x, y))
	{
		SDL_ShowCursor(SDL_DISABLE);
		circleRGBA(r, x, y, parent->get_brush_radius(), 
			       0, 0, 0, 255);
	}
	else 
		SDL_ShowCursor(SDL_ENABLE);
}
