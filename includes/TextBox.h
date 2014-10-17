
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
	bool send;
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
	int get_current_line() const { return current_line; }
	int get_cursor_pos() const { return cursor_pos; }

	void set_text(SDL_Renderer *renderer, std::string t) { surface.set_text(renderer, t); }
	void set_password_box(bool b) { is_password_box = b; surface.set_password_box(b); }
	void set_page(int p) { page = p; }
	void set_send(bool s) { send = s; }
	void set_parent(Canvas * p) { parent = p; }

	void move_cursor_left();
	void move_cursor_right();
	void move_cursor_up();
	void move_cursor_down();
	void delete_char();
	void delete_char(int l, int p)
	{
		current_line = l;
		cursor_pos = p;
		delete_char();
	}

	void insert_char(char c);
	void insert_char(int l, int p, char c)
	{
		current_line = l;
		cursor_pos = p;
		insert_char(c);
	}

	void to_next_line();
	void to_next_line(int l, int p)
	{
		current_line = l;
		cursor_pos = p;
		to_next_line();
	}

	void set_dimensions(int _w, int _h)
	{
		bounding_rect.w = _w;
		bounding_rect.h = _h;

		surface.set_dimensions(_w, _h);
	}

	std::string text() const { return surface.text(); }
};

#endif 
