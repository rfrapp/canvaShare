
#ifndef MENU_H
#define MENU_H

#include <string>
#include "CanvasItem.h"

class Menu
{
public:
	Menu() {  }
	~Menu() {  }

	virtual void get_notification(std::string event, int id) { }
	virtual int get_width() const = 0;
	virtual int get_height() const = 0;
};

#endif 
