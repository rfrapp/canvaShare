
#ifndef TEXTBOXTOOL_H
#define TEXTBOXTOOL_H

#include "Tool.h"
#include "CanvasItem.h"

class Canvas;

class TextBoxTool : public Tool 
{
private:
	CanvasItem * item;
	bool clicked;
	int  pointer_width;

public:
	TextBoxTool(Canvas *c, Rect r);
	~TextBoxTool() { if (item != NULL) delete item; }
	void handle_input(SDL_Event *e);
	void draw(SDL_Renderer *r);
	void toggle_activate();
};

#endif 
