
#include "Button.h"

void Button::draw()
{
	Control::draw();

	// draw the bounding rect
	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 0xFF);
	
	// temporary vars to hold the rects needed 
	// for drawing the button
	SDL_Rect r = bounding_rect.getSDL_Rect();
	SDL_Rect clip = img_clip.getSDL_Rect();
	SDL_Rect o_rect; 

	// draw the background rectangle on the screen
	SDL_RenderFillRect(renderer, &r);

	// if the button was clicked
	if (focus)
	{
		o_rect.x = bounding_rect.x - 2;
		o_rect.y = bounding_rect.y - 2;
		o_rect.w = bounding_rect.w + 4;
		o_rect.h = bounding_rect.h + 4;

		// std::cout << bounding_rect.x << " " << bounding_rect.y << ' ' << bounding_rect.w << ' ' << bounding_rect.h << '\n';

		// draw the outline rectangle in an inverted
		// color from the BG
		SDL_SetRenderDrawColor(renderer, 255 - bg_r, 255 - bg_g, 255 -bg_b, 255);

		SDL_RenderFillRect(renderer, &o_rect);
	}

	if (show_tooltip)
	{
		SDL_Rect t_rect = {bounding_rect.x - 5, bounding_rect.y + bounding_rect.h + 5, 
		                   tooltip_font->get_width(tooltip.c_str()) + 5, tooltip_font->get_line_height()};
		
		if (t_rect.x < 0)
			t_rect.x += -t_rect.x * 2;
		if (t_rect.x + t_rect.w > parent->get_width())
			t_rect.x -= parent->get_width() - (t_rect.x + t_rect.w) - 5;

		if (t_rect.y + t_rect.h > parent->get_height())
			t_rect.y = bounding_rect.y - t_rect.h - 5;

		SDL_Rect t_o_rect = {t_rect.x - 1, t_rect.y - 1, t_rect.w + 2, t_rect.h + 2};

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderFillRect(renderer, &t_o_rect);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderFillRect(renderer, &t_rect);

		tooltip_texture.render(renderer, t_rect.x + 2, t_rect.y);
	}

	// draw the image on the screen
	bg_img.render(renderer, bounding_rect.x, bounding_rect.y, &clip);

}

void Button::handle_input(SDL_Event *e)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (bounding_rect.collide_point(x, y))
	{
		if (!tooltip_timer.isStarted())
			tooltip_timer.start();
	}
	else
	{
		tooltip_timer.stop();
		tooltip_timer.reset();
		show_tooltip = false;
	}

	if (tooltip_timer.getTicks() >= 2000)
	{
		show_tooltip = true;
	}

	if (e->type == SDL_MOUSEBUTTONUP)
	{
		if (bounding_rect.collide_point(x, y))
		{
			// TODO: call callback click function
			focus = (focus ? false : true);
			
			if (parent != NULL)
				parent->get_notification("click", id);
		} 
	}
}
