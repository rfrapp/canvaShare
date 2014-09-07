
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

		std::cout << bounding_rect.x << " " << bounding_rect.y << ' ' << bounding_rect.w << ' ' << bounding_rect.h << '\n';

		// draw the outline rectangle in an inverted
		// color from the BG
		SDL_SetRenderDrawColor(renderer, 255 - bg_r, 255 - bg_g, 255 -bg_b, 255);

		SDL_RenderFillRect(renderer, &o_rect);
	}

	// draw the image on the screen
	bg_img.render(renderer, bounding_rect.x, bounding_rect.y, &clip);

}

void Button::handle_input(SDL_Event *e)
{
	if (e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		if (bounding_rect.collide_point(x, y))
		{
			// TODO: call callback click function

			focus = (focus ? false : true);
		} 
	}
}
