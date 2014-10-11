
#include "PreviousPageTool.h"
#include "Canvas.h"

NextPageTool::NextPageTool(Canvas *c, Rect r)
              : Tool(c, r)
{
}

void NextPageTool::handle_input(SDL_Event *e)
{
}

void NextPageTool::draw(SDL_Renderer *r)
{
}

void NextPageTool::toggle_activate()
{
	Tool::toggle_activate();

    parent->page_right();
    active = false;
}
