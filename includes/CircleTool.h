
#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "Tool.h"
#include "CanvasItem.h"

class Canvas;

class CircleTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;
	int  pointer_width;

public:
	CircleTool(Canvas *c, Rect r);
	~CircleTool() { if (item != NULL) delete item; }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
