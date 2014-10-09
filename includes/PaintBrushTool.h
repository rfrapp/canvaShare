
#ifndef PAINTBRUSHTOOL_H
#define PAINTBRUSHTOOL_H

#include "Tool.h"
#include "CanvasItem.h"
#include "SDL2_gfxPrimitives.h"

class Canvas;

class PaintBrushTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;

public:
	PaintBrushTool(Canvas * p, Rect r);

	~PaintBrushTool() { if (item != NULL) delete item; }
	
	void handle_input(SDL_Event *e);
	
	void draw(SDL_Renderer *r);

	void toggle_activate() 
	{
		Tool::toggle_activate();
	}

};

#endif 	
