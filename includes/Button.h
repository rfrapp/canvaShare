
#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

#include "Control.h"
#include "Texture.h"

class Button : public Control 
{
private:
	std::string label;
	int id; 
	Texture bg_img;
	Rect    img_clip;

public:
	Button( SDL_Renderer * r, int w, int h, int x = 0, int y = 0, 
		    const char * l = "", const char * img_path = "", 
		    int clip_x = 0, int clip_y = 0, int clip_w = 0, int clip_h = 0,
		    Uint8 b_r = 255, Uint8 b_g = 255, Uint8 b_b = 255,
		    Uint8 f_r = 0,   Uint8 f_g = 0,   Uint8 f_b = 0)

			: Control(r, w, h, x, y, b_r, b_g, b_b, f_r, f_g, f_b),
			  label(l), bg_img(Texture()), 
			  img_clip(Rect(clip_x, clip_y, clip_w, clip_h))
	{
		bg_img.load_from_file(renderer, img_path, 255, 255, 255);
	}

	void draw();

	std::string get_label()  const { return label;  }
	Texture     get_bg_img() const { return bg_img; }

	void set_label(std::string s) { label = s;    }
	void set_pos(int x, int y) { bounding_rect.x = x; bounding_rect.y = y; }
	void handle_input(SDL_Event *e);
};

#endif 
