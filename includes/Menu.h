
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
	virtual int get_brush_radius() const = 0;
	virtual Uint8 get_foreground_r() const = 0;
	virtual Uint8 get_foreground_g() const = 0;
	virtual Uint8 get_foreground_b() const = 0;
	virtual Uint8 get_foreground_a() const = 0;

	virtual Uint8 get_background_r() const = 0;
	virtual Uint8 get_background_g() const = 0;
	virtual Uint8 get_background_b() const = 0;
	virtual Uint8 get_background_a() const = 0;
	virtual void increment_brush_radius() = 0;
	virtual void decrement_brush_radius() = 0;
	virtual void add_canvas_item(const CanvasItem & i) = 0;
	virtual std::vector< CanvasItem > & get_items() = 0;
};

#endif 
