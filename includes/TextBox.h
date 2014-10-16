
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Control.h"
#include "Rect.h"
#include "TextSurface.h"
#include "Font.h"

class Canvas;

class TextBox : public Control 
{
private:
	Canvas * parent;
	TextSurface surface;
	Rect cursor_rect;
	int cursor_pos;
	int current_line; 
	bool outline;
	bool multiline;
	bool is_password_box;
	int page;

	Uint8 outline_r, outline_g, outline_b;

public:
	TextBox(SDL_Renderer *r, Font *f, int w, int h, int x = 0, int y = 0,
					  bool o = true, bool multi_line = true,
		              Uint8 b_r = 255, Uint8 b_g = 255, Uint8 b_b = 255,
		              Uint8 f_r = 0,   Uint8 f_g = 0,   Uint8 f_b = 0,
		              Uint8 o_r = 0xBB, Uint8 o_g = 0xBB, Uint8 o_b = 0xBB);

	void draw();

	void handle_input(SDL_Event *e);

	int get_page() const { return page; }

	void set_text(SDL_Renderer *renderer, std::string t) { surface.set_text(renderer, t); }
	void set_password_box(bool b) { is_password_box = b; surface.set_password_box(b); }
	void set_page(int p) { page = p; }

	void set_parent(Canvas * p) { parent = p; }

	void set_dimensions(int _w, int _h)
	{
		bounding_rect.w = _w;
		bounding_rect.h = _h;

		surface.set_dimensions(_w, _h);
	}

	std::string text() const { return surface.text(); }
};

#endif 
