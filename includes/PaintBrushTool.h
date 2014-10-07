
#ifdef PAINTBRUSHTOOL_H
#define PAINTBRUSHTOOL_H

#include "Tool.h"

class PaintBrushTool : public Tool 
{
private:
	struct Pos 
	{
		int x, y;
	};

	std::vector< Pos > tmp;
	std::vector< std::vector< Pos > > lines;
	int brush_radius; 

public:
	PaintBrushTool(Menu * p, std::string c)
	: Tool(p, c), brush_radius(1)
	{
	}

	~PaintBrushTool();
	
	void handle_input(SDL_Event *e);
	void draw();
	void on_activate();

};

#endif 	
