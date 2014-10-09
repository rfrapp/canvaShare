
#ifndef TRIANGLETOOL_H
#define TRIANGLETOOL_H

#include "Tool.h"
#include "CanvasItem.h"

class Canvas;

class TriangleTool : public Tool 
{
private:
	CanvasItem *item;
	bool clicked;
	int  pointer_width;

public: 
	TriangleTool(Canvas *c, Rect r);
	~TriangleTool() { if (item != NULL) delete item; }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif
