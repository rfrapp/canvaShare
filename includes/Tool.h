
#ifndef TOOL_H
#define TOOL_H 

#include "Menu.h"

class Tool
{
protected:
	std::string cursor;
	Menu * parent; 

public:
	Tool(Menu * p, std::string c) : parent(p), cursor(c)
	{
	}

	~Tool() {  }

	virtual void draw() = 0;
	virtual void on_activate() = 0;
	virtual void handle_input(SDL_Event *e) = 0;
};

#endif 
