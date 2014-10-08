
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
};

#endif 
