
#include "ScreenshotTool.h"
#include "Canvas.h"

ScreenshotTool::ScreenshotTool(Canvas *c, Rect r)
              : Tool(c, r)
{
}

void ScreenshotTool::handle_input(SDL_Event *e)
{
}

void ScreenshotTool::draw(SDL_Renderer *r)
{
}

void ScreenshotTool::toggle_activate()
{
	Tool::toggle_activate();

	SDL_Surface *sshot = SDL_CreateRGBSurface(0, parent->get_width(), parent->get_height()	, 32, 
		                                      0x00ff0000, 0x0000ff00, 0x000000ff, 
		                                      0xff000000);
	SDL_Rect rect = draw_bounds.getSDL_Rect();
    SDL_RenderReadPixels(parent->get_renderer(), &rect, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "screenshot.bmp");
    SDL_FreeSurface(sshot);

    active = false;
}
