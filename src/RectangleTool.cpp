
#include "RectangleTool.h"
#include "Canvas.h"

RectangleTool::RectangleTool(Canvas *c, Rect r)
              : Tool(c, r), clicked(false),
                pointer_width(5), item(NULL)
{
}

void RectangleTool::handle_input(SDL_Event *e)
{
	if (active)
	{
		int x, y;
   		SDL_GetMouseState(&x, &y);
    	if (e->type == SDL_MOUSEBUTTONDOWN && draw_bounds.collide_point(x, y))
    	{
    		if (item != NULL) delete item; 
    		item = new CanvasItem("rect", 
							  parent->get_foreground_r(), parent->get_foreground_g(),
							  parent->get_foreground_b(), parent->get_foreground_a(),
			                  parent->get_background_r(), parent->get_background_g(),
			                  parent->get_background_b(), parent->get_background_a(), 
			                  parent->get_brush_radius());

    		Point p = {x, y};
    		clicked = true;

    		item->points.push_back(p);
    		parent->add_canvas_item(*item);
    	}
    	else if (e->type == SDL_MOUSEMOTION && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
    			// if (parent->get_items()[parent->get_items().size() - 1].points.size() == 2)
    			// 	parent->get_items()[parent->get_items().size() - 1].points.pop_back();

    			Point p = {x, y};
                parent->add_point_to_item(p);
    			//parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
    	else if (e->type == SDL_MOUSEBUTTONUP && draw_bounds.collide_point(x, y))
    	{
    		if (clicked)
    		{
    			// if (parent->get_items()[parent->get_items().size() - 1].points.size() == 2)
    			// 	parent->get_items()[parent->get_items().size() - 1].points.pop_back();

	    		Point p = {x, y};

	    		clicked = false;
	    		parent->add_point_to_item(p);
                //parent->get_items()[parent->get_items().size() - 1].points.push_back(p);
    		}
    	}
	}
}

void RectangleTool::draw(SDL_Renderer *r)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (active && draw_bounds.collide_point(x, y))
	{
		SDL_ShowCursor(SDL_DISABLE);
		hlineRGBA(r, x - pointer_width, x + pointer_width, y, 0, 0, 0, 255);
		vlineRGBA(r, x, y - pointer_width, y + pointer_width, 0, 0, 0, 255);
	}
	else 
		SDL_ShowCursor(SDL_ENABLE);
}

void RectangleTool::toggle_activate()
{
	Tool::toggle_activate();
}
