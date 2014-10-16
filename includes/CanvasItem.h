
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
		       int radius=1, int pg=0) : type(t), foreground_r(fg_r), foreground_g(fg_g),
			   foreground_b(fg_b), foreground_a(fg_a), background_r(bg_r), background_g(bg_g),
			   background_b(bg_b), background_a(bg_a), page(pg), brush_radius(radius)
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

	void move(int dx, int dy)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points[i].x += dx;
			points[i].y += dy;
		}
	}

	void set_page(int p) { page = p; }
	void set_brush_radius(int r) { brush_radius = r; }
	void set_points(std::vector< Point > p) { points = p; }

	static CanvasItem string_to_item(std::string str)
	{
		int l = 0;
		int br = 0, page = 0;
		Uint8 fr, fg, fb, fa;
		Uint8 bgr, bg, bb, ba;
		std::string type = "";
		std::vector < Point > points;
		Point tmp;

		std::stringstream stream;
		std::string line;
		stream << str;

		while (getline(stream, line))
		{
			if (l == 0)
				br = atoi(line.c_str());
			else if (l == 1)
				type = line.c_str();
			else if (l == 2)
				page = atoi(line.c_str());
			else if (l == 3)
				fr = atoi(line.c_str());
			else if (l == 4)
				fg = atoi(line.c_str());
			else if (l == 5)
				fb = atoi(line.c_str());
			else if (l == 6)
				fa = atoi(line.c_str());
			else if (l == 7)
				bgr = atoi(line.c_str());
			else if (l == 8)
				bg = atoi(line.c_str());
			else if (l == 9)
				bb = atoi(line.c_str());
			else if (l == 10)
				ba = atoi(line.c_str());
			else
			{
				if (l % 2 == 1)
					tmp.x = atoi(line.c_str());
				else
				{
					tmp.y = atoi(line.c_str());
					points.push_back(tmp);
				}
			}

			l++;
		}

		CanvasItem item(type, fr, fg, fb, fa, bgr, bg, bb, ba, br, page);
		item.set_points(points);
		return item;
	}

	std::string to_string() const 
	{
		std::string str;
		std::stringstream stream;
		stream << brush_radius << '\n'
		       << type << '\n'
		       << page << '\n'
		       << (int)foreground_r << '\n'
		       << (int)foreground_g << '\n'
		       << (int)foreground_b << '\n'
		       << (int)foreground_a << '\n'
		       << (int)background_r << '\n'
		       << (int)background_g << '\n'
		       << (int)background_b << '\n'
		       << (int)background_a << '\n';

		for (int i = 0; i < points.size(); i++)
		{
			stream << points[i].x << '\n' 
				   << points[i].y << '\n';
		}

		str = stream.str();

		return str;
	}
};

#endif 
