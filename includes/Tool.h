
#ifndef TOOL_H
#define TOOL_H 

#include "Rect.h"
class Canvas;

class Tool
{
protected:
	Canvas * parent; 
	bool active;
	Rect draw_bounds;

public:
	Tool(Canvas * p, Rect r) : parent(p), active(false),
	                         draw_bounds(r)
	{
	}
	
	~Tool() {  }

	virtual void draw(SDL_Renderer *r) = 0;
	virtual void toggle_activate() = 0;
	virtual void handle_input(SDL_Event *e) = 0;
};

#endif 
