
#include "RectangleTool.h"
#include "Canvas.h"

UndoTool::UndoTool(Canvas *c, Rect r)
              : Tool(c, r)
{
}

void UndoTool::handle_input(SDL_Event *e)
{
}

void UndoTool::draw(SDL_Renderer *r)
{
}

void UndoTool::toggle_activate()
{
	Tool::toggle_activate();

    parent->undo_canvas_item();
    active = false;
}
