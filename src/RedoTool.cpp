
#include "RectangleTool.h"
#include "Canvas.h"

RedoTool::RedoTool(Canvas *c, Rect r)
              : Tool(c, r)
{
}

void RedoTool::handle_input(SDL_Event *e)
{
}

void RedoTool::draw(SDL_Renderer *r)
{
}

void RedoTool::toggle_activate()
{
	Tool::toggle_activate();

    parent->redo_canvas_item();
    active = false;
}
