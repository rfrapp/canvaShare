

#include "Canvas.h"
#include "PencilTool.h"

PencilTool::PencilTool(Canvas * p, Rect r)
			   : Tool(p, r), item(NULL), clicked(false)
{
}

void PencilTool::handle_input(SDL_Event *e) 
{ 
	if (active)
	{
		int x, y;
   		SDL_GetMouseState(&x, &y);
    	
    	if (e->type == SDL_MOUSEBUTTONDOWN && draw_bounds.collide_point(x, y))
    	{
			if (item != NULL) delete item; 

    		item = new CanvasItem("pencil", 
							  parent->get_foreground_r(), parent->get_foreground_g(),
							  parent->get_foreground_b(), parent->get_foreground_a(),
			                  parent->get_background_r(), parent->get_background_g(),
			                  parent->get_background_b(), parent->get_background_a(), 
			                  0);

    		Point p = {x, y};
    		clicked = true;

    		item->points.push_back(p);
    		parent->add_canvas_item(*item);
    	}
    	else if (e->type == SDL_MOUSEMOTION && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
	    		Point p = {x, y};
    			parent->add_point_to_item(p);
                // parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
    	else if (e->type == SDL_MOUSEBUTTONUP && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
	    		Point p = {x, y};

	    		clicked = false;
	    		parent->add_point_to_item(p);
                // parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
	}
}

void PencilTool::draw(SDL_Renderer *r)
{
}

