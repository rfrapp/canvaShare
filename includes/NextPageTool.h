
#ifndef NEXTPAGETOOL_H
#define NEXTPAGETOOL_H

#include "Tool.h"

class Canvas;

class NextPageTool : public Tool 
{
public:
	NextPageTool(Canvas *c, Rect r);
	~NextPageTool() {  }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
