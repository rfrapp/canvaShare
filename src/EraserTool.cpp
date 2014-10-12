
#include "Canvas.h"
#include "PaintBrushTool.h"

EraserTool::EraserTool(Canvas * p, Rect r)
			   : Tool(p, r), item(NULL), clicked(false)
{
}

void EraserTool::handle_input(SDL_Event *e) 
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

    		item = new CanvasItem("erase", 
							  parent->get_foreground_r(), parent->get_foreground_g(),
							  parent->get_foreground_b(), parent->get_foreground_a(),
			                  parent->get_background_r(), parent->get_background_g(),
			                  parent->get_background_b(), parent->get_background_a(), 
			                  parent->get_brush_radius());

    		Point p = {x, y};
    		clicked = true;

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
    			parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
    	else if (e->type == SDL_MOUSEBUTTONUP && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
	    		Point p = {x, y};

	    		clicked = false;
	    		parent->get_items()[parent->get_items().size() - 1].set_brush_radius(parent->get_brush_radius());
	    		parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
	}
}

void EraserTool::draw(SDL_Renderer *r)
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
