
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
	        	if (cursor_pos == 0 && current_line == 9)
	        		return;

	            // lop off character
	            cursor_pos--;
	            surface.remove_char_at(renderer, cursor_pos, current_line);

	            std::string c = "a";

	            cursor_rect.x = bounding_rect.x + cursor_pos * surface.get_font()->get_width(c.c_str());
	            cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
	        }

	        if (e->key.keysym.sym == SDLK_RETURN)
	        {
	        	// Add a new line to the TextSurface
	        	surface.add_newline(renderer, cursor_pos, current_line);

	        	// Set the cursor to the TextBox's far-left
	        	// bound and add the length of a line to the
	        	// cursor's y 
	        	cursor_rect.x = bounding_rect.x; 
	        	cursor_rect.y += surface.get_font()->get_line_height();
	        	current_line++;
	        	cursor_pos = 0; 	
	        }

	        if (e->key.keysym.sym == SDLK_LEFT)
	        {
        		// If the cursor is not at the far
        		// left of a line
	        	if (cursor_pos > 0)
	        	{
	        		std::string c = "a";

	        		// Subtract the width of that character from the 
	        		// cursor rectangle's position
	        		cursor_rect.x -= surface.get_font()->get_width(c.c_str());

	        		cursor_pos--;
	        	}
	        	else // The user hit the left arrow at the beginning of a line
	        	{
        			if (current_line > 0)
        			{
        				current_line--;
        				std::string line = surface.get_lines()[current_line];
        				cursor_pos = line.length();

        				cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(line.c_str());
        				cursor_rect.y -= surface.get_font()->get_line_height();
        			}
        		}
	        }

	        if (e->key.keysym.sym == SDLK_RIGHT)
	        {
	        	if (cursor_pos < surface.length())
	        	{
	        		// Advance the cursor 1 further in 
	        		// the string
	        		cursor_pos++;

	        		// Get the text for the current line 
	        		std::string line = surface.get_lines()[current_line];

	        		// Check if the user pressed the right arrow key
	        		// and the cursor is at the end of the current
	        		// line 
	        		if (cursor_rect.x == bounding_rect.x + surface.get_font()->get_width(line.c_str()))
	        		{
	        			// Change the current line to the next one
	        			current_line++;

	        			// Subtract one from the cursor position to
	        			// compensate for the line change
	        			cursor_pos--;

	        			// Set the cursor x to the left bound of the
	        			// textbox
	        			cursor_rect.x = bounding_rect.x;

	        			// Set the cursor y to the y value of the 
	        			// current line 
	        			cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
	        		}
	        		else // If the cursor is not at the right end of the line
	        		{
	        			// Get the character before the current position 
	        			std::string c = surface.get_char_at_index(cursor_pos - 1);

	        			// Add the width of that character to the cursor's x 
	        			// value 
	        			cursor_rect.x += surface.get_font()->get_width(c.c_str());
	        		}

	        		std::cout << "cursor_pos in right: " << cursor_pos << std::endl;
	        	}
	        }
		}
		// Check if the user entered text
	    else if( e->type == SDL_TEXTINPUT )
	    {
	        // Not copy or pasting
	        if( !( ( e->text.text[ 0 ] == 'c' || e->text.text[ 0 ] == 'C' ) && ( e->text.text[ 0 ] == 'v' || e->text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
	        {
	        	std::cout << "cursor_pos before insert(): " << cursor_pos << std::endl;
	        	std::cout << "current_line before insert(): " << current_line << std::endl;

	            // Append character at the cursor position
	            surface.insert_text_at_index(renderer, e->text.text, cursor_pos, current_line);
	            
	            cursor_pos++;

	            cursor_rect.x = bounding_rect.x + cursor_pos * surface.get_font()->get_width(&e->text.text[0]);
	            cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();

	            std::cout << "cursor_pos after text input: " << cursor_pos << std::endl;

	        }
	    }
	}
};

#endif 
