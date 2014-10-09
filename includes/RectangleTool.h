
#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "Tool.h"
#include "CanvasItem.h"

class Canvas;

class RectangleTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;
	int  pointer_width;

public:
	RectangleTool(Canvas *c, Rect r);
	~RectangleTool() { if (item != NULL) delete item; }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
