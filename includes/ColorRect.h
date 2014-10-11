
#ifndef COLORRECT_H
#define COLORRECT_H

#include "Rect.h"
#include <string>

class ColorRect : public Rect 
{
public:
	int r, g, b;

	ColorRect() {  }

	ColorRect(int x0, int y0, int w0, int h0, int r0, int g0, int b0)
	: Rect(x0, y0, w0, h0), r(r0), g(g0), b(b0)
	{
	}

	~ColorRect() {  }
	
};

#endif 
