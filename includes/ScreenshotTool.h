
#ifndef SCREENSHOTTOOL_H
#define SCREENSHOTTOOL_H

#include "Tool.h"

class Canvas;

class ScreenshotTool : public Tool 
{
public:
	ScreenshotTool(Canvas *c, Rect r);
	~ScreenshotTool() {  }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
