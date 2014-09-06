
#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>
#include "Rect.h"

class Control
{
private:

	// 8-bit unsigned integers for
	// r, g, b values for the control's background
	// color
	Uint8 bg_r, bg_g, bg_b;

	// 8-bit unsigned integers for
	// r, b, b values for the control's foreground
	// color
	Uint8 fg_r, fg_g, gf_b;

	// A rectangle that outlines the control
	Rect  bounding_rect;

	// Boolean to designate wheter or not the 
	// control has mouse focus
	bool  focus;

public:

	Control(int width, int height, int x = 0, int y = 0, 
		    Uint8 b_r = 255, Uint8 b_g = 255, Uint8 b_b = 255,
		    Uint8 f_r = 0,   Uint8 f_g = 0,   Uint8 f_b = 0)

	       : bounding_rect(Rect(x, y, width, height)),
	         focus(false), bg_r(b_r), bg_g(b_g), bg_b(b_b),
	         fg_r(f_r), fg_g(f_g), fg_b(f_b)
	{
	}

	// Draw the control. Can be polymorphically 
	// overridden to draw differently for differen
	// controls 
	virtual void draw();

	// Handle input while the control has focus. Can be
	// overridden to handle different inputs differently
	// depending on the control 
	virtual void handle_input();

	// ========================================================================
	// Getters 
	// ========================================================================
	bool         has_focus()     const { return focus;         }
	Rect         rect()          const { return bounding_rect; }
	// ========================================================================


	// ========================================================================
	// Setters
	// ========================================================================
	
	// ========================================================================
};

#endif 
