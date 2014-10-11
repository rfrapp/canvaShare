
#include "PreviousPageTool.h"
#include "Canvas.h"

PreviousPageTool::PreviousPageTool(Canvas *c, Rect r)
              : Tool(c, r)
{
}

void PreviousPageTool::handle_input(SDL_Event *e)
{
}

void PreviousPageTool::draw(SDL_Renderer *r)
{
}

void PreviousPageTool::toggle_activate()
{
	Tool::toggle_activate();

    parent->page_left();
    active = false;
}
