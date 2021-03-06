
#ifndef LOGINMENU_H
#define LOGINMENU_H 

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "Menu.h"
#include "Button.h"
#include "TextBox.h"
#include "TextSurface.h"

class Program;

class LoginMenu : public Menu 
{
private:
	int w, h;
	SDL_Renderer * renderer;

	Font * font;
	Font * title_font; 
	Button * login_button;
	TextBox * username_box; 
	TextBox * password_box;
	TextSurface * username_label;
	TextSurface * password_label;
	TextSurface * title_surface;
	TextSurface * message_surface;

	Program * parent;
	Button * register_button;

	bool show_message;
	bool logged_in;

public:
	LoginMenu(Program *p, SDL_Renderer * r, int _w, int _h);

	void draw();
	void handle_input(SDL_Event * e);

	bool load_media();
	void init_controls();
	void get_notification(std::string event, int id);
	void receive_message(std::string str);

	void authenticate();
	void register_user();

	~LoginMenu()
	{
		if (login_button != NULL)
			delete login_button;

		if (username_box != NULL)
			delete username_box;

		if (password_box != NULL)
			delete password_box;

		if (username_label != NULL)
			delete username_label;

		if (password_label != NULL)
			delete password_label;
	}

	bool is_logged_in() const { return logged_in; }
	int get_width() const { return w; }
	int get_height() const { return h; }
	void set_renderer(SDL_Renderer *r) { renderer = r; }
	
};

#endif 
