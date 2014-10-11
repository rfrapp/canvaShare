
#ifndef PREVIOUSPAGETOOL_H
#define PREVIOUSPAGETOOL_H

#include "Tool.h"

class Canvas;

class PreviousPageTool : public Tool 
{
public:
	PreviousPageTool(Canvas *c, Rect r);
	~PreviousPageTool() {  }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
