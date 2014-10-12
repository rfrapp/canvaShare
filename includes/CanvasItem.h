
#ifndef CANVASITEM_H
#define CANVASITEM_H

#include <vector>
#include <string>
#include <sstream>
#include "Rect.h"

struct Point 
{
	int x, y;
};

class CanvasItem
{
private:
	int brush_radius;
	int page;
	std::string type;
	Uint8 foreground_r;
	Uint8 foreground_g;
	Uint8 foreground_b;
	Uint8 foreground_a;
	Uint8 background_r;
	Uint8 background_g;
	Uint8 background_b;
	Uint8 background_a;

public:
	std::vector< Point > points; 
	CanvasItem(std::string t, Uint8 fg_r, Uint8 fg_g, Uint8 fg_b, Uint8 fg_a, 
		       Uint8 bg_r, Uint8 bg_g, Uint8 bg_b, Uint8 bg_a, 
		       int brush_radius=1) : type(t), foreground_r(fg_r), foreground_g(fg_g),
			   foreground_b(fg_b), foreground_a(fg_a), background_r(bg_r), background_g(bg_g),
			   background_b(bg_b), background_a(bg_a)
	{ 
	}
	
	~CanvasItem() {  }
	Uint8 get_foreground_r() const { return foreground_r; }
	Uint8 get_foreground_g() const { return foreground_g; }
	Uint8 get_foreground_b() const { return foreground_b; }
	Uint8 get_foreground_a() const { return foreground_a; }

	Uint8 get_background_r() const { return background_r; }
	Uint8 get_background_g() const { return background_g; }
	Uint8 get_background_b() const { return background_b; }
	Uint8 get_background_a() const { return background_a; }

	std::string get_type() const { return type; }
	int get_brush_radius() const { return brush_radius; }
	int get_page() const { return page; }

	Rect get_bounding_rect()
	{
		int minx = points[0].x;
		int maxx = points[0].x;
		int miny = points[0].y;
		int maxy = points[0].y;

		// Get the minimum X value for the left
		// side of the rect
		for (int i = 0; i < points.size(); i++)
		{
			if (points[i].x < minx)
				minx = points[i].x;
		}

		// Get the maximum X value for the right
		// side of the rect
		for (int i = 0; i < points.size(); i++)
		{
			if (points[i].x > maxx)
				maxx = points[i].x;
		}

		// Get the minimum Y value for the top 
		// side of the rect
		for (int i = 0; i < points.size(); i++)
		{
			if (points[i].y < miny)
				miny = points[i].y;
		}

		// Get the maximum Y value for the bottom
		// side of the rect
		for (int i = 0; i < points.size(); i++)
		{
			if (points[i].y > maxy)
				maxy = points[i].y;
		}

		if (type == "paint")
		{
			minx -= brush_radius;
			maxx += brush_radius;
			miny -= brush_radius;
			maxy += brush_radius;
		}

		return Rect(minx, miny, maxx - minx, maxy - miny);
	}

	void set_page(int p) { page = p; }
	void set_brush_radius(int r) { brush_radius = r; }
	
	std::string to_string() const 
	{
		std::string str;
		std::stringstream stream;
		stream << brush_radius << '\n'
		       << type << '\n'
		       << page << '\n'
		       << foreground_r << '\n'
		       << foreground_g << '\n'
		       << foreground_b << '\n'
		       << foreground_a << '\n'
		       << background_r << '\n'
		       << background_g << '\n'
		       << background_b << '\n'
		       << background_a << '\n';

		for (int i = 0; i < points.size(); i++)
		{
			stream << points[i].x 
				   << points[i].y;
		}

		str = stream.str();

		return str;
	}
};

#endif 
