
#ifndef PENCILTOOL_H
#define PENCILTOOL_H

#include "Tool.h"
#include "CanvasItem.h"
#include "SDL2_gfxPrimitives.h"

class Canvas;

class PencilTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;

public:
	PencilTool(Canvas * p, Rect r);

	~PencilTool() { if (item != NULL) delete item; }
	
	void handle_input(SDL_Event *e);
	
	void draw(SDL_Renderer *r);

	void toggle_activate() 
	{
		Tool::toggle_activate();
	}

};

#endif 	
