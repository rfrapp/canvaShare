
#ifndef UNDOTOOL_H
#define UNDOTOOL_H

#include "Tool.h"

class Canvas;

class UndoTool : public Tool 
{
public:
	UndoTool(Canvas *c, Rect r);
	~UndoTool() {  }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
