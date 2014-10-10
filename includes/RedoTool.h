
#ifndef REDOTOOL_H
#define REDOTOOL_H

#include "Tool.h"

class Canvas;

class RedoTool : public Tool 
{
public:
	RedoTool(Canvas *c, Rect r);
	~RedoTool() {  }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
