
#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "Tool.h"
#include "CanvasItem.h"
#include "SDL2_gfxPrimitives.h"

class Canvas;

class EraserTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;

public:
	EraserTool(Canvas * p, Rect r);

	~EraserTool() { if (item != NULL) delete item; }
	
	void handle_input(SDL_Event *e);
	
	void draw(SDL_Renderer *r);

	void toggle_activate() 
	{
		Tool::toggle_activate();
	}

};

#endif 	
