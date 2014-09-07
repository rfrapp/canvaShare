
#include "Button.h"

void Button::draw()
{
	Control::draw();

	// draw the bounding rect
	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 0xFF);
	
	SDL_Rect r = bounding_rect.getSDL_Rect();
	SDL_Rect clip = img_clip.getSDL_Rect();

	SDL_RenderFillRect(renderer, &r);
	bg_img.render(renderer, bounding_rect.x, bounding_rect.y, &clip);

}
