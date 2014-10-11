
#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "Menu.h"
#include "Control.h"
#include "Texture.h"
#include "Timer.h"
#include "TextSurface.h"

class Button : public Control 
{
private:
	Menu * parent;
	std::string tooltip;
	int id; 
	Texture bg_img;
	Rect    img_clip;
	Timer tooltip_timer;
	bool show_tooltip;
	TextSurface tooltip_texture;
	Font * tooltip_font;

public:
	Button( int _id, Menu * p, SDL_Renderer * r, Font * t_font, int w, int h, int x = 0, int y = 0, 
		    const char * tip = "", const char * img_path = "", 
		    int clip_x = 0, int clip_y = 0, int clip_w = 0, int clip_h = 0,
		    Uint8 b_r = 255, Uint8 b_g = 255, Uint8 b_b = 255,
		    Uint8 f_r = 0,   Uint8 f_g = 0,   Uint8 f_b = 0)

			: Control(r, w, h, x, y, b_r, b_g, b_b, f_r, f_g, f_b),
			  tooltip(tip), bg_img(Texture()), 
			  img_clip(Rect(clip_x, clip_y, clip_w, clip_h)),
			  id(_id), parent(p), tooltip_font(t_font),
			  tooltip_texture(t_font, 150, 0, 0, 0, false)
	{
		if (img_path != "")
			bg_img.load_from_file(renderer, img_path, 255, 255, 255);
		tooltip_texture.set_text(r, tip);
	}

	void draw();

	std::string get_tooltip()  const { return tooltip;  }
	Texture     get_bg_img() const { return bg_img; }

	void set_tooltip(std::string s) { tooltip = s;    }
	void set_pos(int x, int y) { bounding_rect.x = x; bounding_rect.y = y; }
	void handle_input(SDL_Event *e);
};

#endif 
