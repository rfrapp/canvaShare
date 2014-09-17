
#ifndef MULTILINE_TEXTBOX_H
#define MULTILINE_TEXTBOX_H

#include "Control.h"
#include "Rect.h"
#include "TextSurface.h"
#include "Font.h"

class Multiline_TextBox : public Control 
{
private:
	TextSurface surface;
	Rect cursor_rect;
	int cursor_pos;
	int current_line; 
	bool outline;

	Uint8 outline_r, outline_g, outline_b;

public:
	Multiline_TextBox(SDL_Renderer *r, Font *f, int w, int h, int x = 0, int y = 0,
					  bool o = true,
		              Uint8 b_r = 255, Uint8 b_g = 255, Uint8 b_b = 255,
		              Uint8 f_r = 0,   Uint8 f_g = 0,   Uint8 f_b = 0,
		              Uint8 o_r = 0xBB, Uint8 o_g = 0xBB, Uint8 o_b = 0xBB)
			
			: Control(r, w, h, x, y, b_r, b_g, b_b, f_r, f_g, f_b),
			  surface(f, w, h, x, y), cursor_rect(x, y, 1, f->get_line_height()),
			  cursor_pos(0), outline(o), outline_r(o_r), outline_g(o_g),
			  outline_b(o_b), current_line(0)
	{
	}

	void draw()
	{
		// draw the bounding rect
		SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 0xFF);
		SDL_Rect rect = bounding_rect.getSDL_Rect();

		SDL_RenderFillRect(renderer, &rect);

		if (outline)
		{
			SDL_SetRenderDrawColor(renderer, outline_r, outline_g, outline_b, 0xFF);
			SDL_RenderDrawRect(renderer, &rect);
		}
		else
			SDL_RenderFillRect(renderer, &rect);

		// draw the cursor rect
		SDL_Rect c_rect = cursor_rect.getSDL_Rect();
		SDL_SetRenderDrawColor(renderer, fg_r, fg_g, fg_b, 0xFF);
		SDL_RenderFillRect(renderer, &c_rect);

		// render the text 
		surface.render(renderer);
	}

	void handle_input(SDL_Event *e)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->key.keysym.sym == SDLK_BACKSPACE && surface.length() > 0)
	        {
	        	std::string c = "";

	        	if (cursor_pos > 0)
	        		c = surface.get_char_at_index(cursor_pos - 1);

	        	std::cout << "here " << cursor_pos << std::endl;

	            // lop off character
	            surface.remove_char_at(renderer, cursor_pos - 1);
	            std::string last_line = "";

	            if (surface.get_lines().size() > 0)
	            	last_line = surface.get_lines()[surface.num_lines() - 1];
	            
	            if (cursor_rect.x == bounding_rect.x)
	            {
	            	// Adjust the cursor position
	            	cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(last_line.c_str());
	            	cursor_rect.y = bounding_rect.y + (surface.num_lines() - 1) * surface.get_font()->get_line_height();
	        	}
	        	else
	        	{
	        		if (cursor_pos > 0)
	        			cursor_rect.x -= surface.get_font()->get_width(c.c_str());
	        		else
	        		{
	        			cursor_rect.x = bounding_rect.x;
	        		}
	        	}

	            if (cursor_pos > 0)
	            	cursor_pos--;
	        }

	        if (e->key.keysym.sym == SDLK_RETURN)
	        {
	        	surface.add_newline();
	        	cursor_rect.x = bounding_rect.x; 
	        	cursor_rect.y += surface.get_font()->get_line_height();
	        }

	        if (e->key.keysym.sym == SDLK_LEFT)
	        {
	        	if (cursor_pos > 0)
        			cursor_pos--;

	        	if (cursor_rect.x != bounding_rect.x)
	        	{
	        		std::string c = surface.get_char_at_index(cursor_pos);
	        		cursor_rect.x -= surface.get_font()->get_width(c.c_str());
	        	}
	        	else 
	        	{
	        		current_line--;
	        		cursor_pos++;

	        		if (current_line >= 0)
		        	{
		        		std::string line = surface.get_lines()[current_line];
		        		cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
		        		cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(line.c_str());
		        	}
	        	}

	        	std::cout << "cursor_pos: " << cursor_pos << std::endl;
	        }

	        if (e->key.keysym.sym == SDLK_RIGHT)
	        {
	        	if (cursor_pos < surface.length())
	        	{
	        		cursor_pos++;
	        		std::string line = surface.get_lines()[current_line];

	        		if (cursor_rect.x == bounding_rect.x + surface.get_font()->get_width(line.c_str()))
	        		{
	        			current_line++;
	        			cursor_pos--;
	        			cursor_rect.x = bounding_rect.x;
	        			cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
	        		}
	        		else
	        		{
	        			std::string c = surface.get_char_at_index(cursor_pos - 1);
	        			cursor_rect.x += surface.get_font()->get_width(c.c_str());
	        		}

	        		std::cout << "cursor_pos in right: " << cursor_pos << std::endl;
	        	}
	        }
		}
		//Special text input event
	    else if( e->type == SDL_TEXTINPUT )
	    {
	        //Not copy or pasting
	        if( !( ( e->text.text[ 0 ] == 'c' || e->text.text[ 0 ] == 'C' ) && ( e->text.text[ 0 ] == 'v' || e->text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
	        {
	            //Append character
	            surface.insert_text_at_index(renderer, e->text.text, cursor_pos);
	            std::string last_line = surface.get_lines()[surface.num_lines() - 1];
	            
	            // Change the x,y position 
	            cursor_rect.x += surface.get_font()->get_width(e->text.text);

	            if (cursor_rect.x > bounding_rect.w)
	            {
	            	cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(last_line.c_str());
	            	cursor_rect.y = bounding_rect.y + (surface.num_lines() - 1) * surface.get_font()->get_line_height();
	            }

	            cursor_pos++;
	            std::cout << "cursor_pos: " << cursor_pos << std::endl;
	            current_line = surface.get_line_for_index(cursor_pos);

	        }
	    }
	}
};

#endif 
