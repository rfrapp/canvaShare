
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

	virtual void draw() {  }
	virtual void on_activate() {  }
	virtual void handle_input(SDL_Event *e) {  }
};

#endif 
