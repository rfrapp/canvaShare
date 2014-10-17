
#include "TextBox.h"
#include "Canvas.h"
#include <sstream>

TextBox::TextBox(SDL_Renderer *r, Font *f, int w, int h, int x, int y,
				  bool o, bool multi_line,
	              Uint8 b_r, Uint8 b_g, Uint8 b_b,
	              Uint8 f_r,   Uint8 f_g, Uint8 f_b,
	              Uint8 o_r, Uint8 o_g, Uint8 o_b)
		
		: Control(r, w, h, x, y, b_r, b_g, b_b, f_r, f_g, f_b),
		  surface(f, w, h, x, y, multi_line, f_r, f_g, f_b), cursor_rect(x, y, 1, f->get_line_height()),
		  cursor_pos(0), outline(o), outline_r(o_r), outline_g(o_g),
		  outline_b(o_b), current_line(0), multiline(multi_line),
		  is_password_box(false), page(-1), parent(NULL), send(false)
{
	if (!multiline)
	{
		bounding_rect.h = surface.get_font()->get_height('c');
	}
}

void TextBox::draw()
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
	if (focus)
	{
		SDL_Rect c_rect = cursor_rect.getSDL_Rect();
		SDL_SetRenderDrawColor(renderer, fg_r, fg_g, fg_b, 0xFF);
		SDL_RenderFillRect(renderer, &c_rect);
	}

	// render the text 
	surface.render(renderer);
}

void TextBox::move_cursor_left()
{
	// If the cursor is not at the far
	// left of a line
	if (cursor_pos > 0)
	{
		std::string c = "a";

		cursor_pos--;

		if (multiline)
		{
			// Subtract the width of that character from the 
			// cursor rectangle's position
			cursor_rect.x -= surface.get_font()->get_width(c.c_str());
		}
		else
		{
			if (cursor_pos == surface.get_visible_char_start() - 1)
    		{
    			surface.set_visible_char_start(cursor_pos);
    			surface.set_visible_char_end(surface.get_visible_char_end() - 1);
    			surface.reload(renderer);

    			cursor_rect.x = bounding_rect.x;
    		}
    		else
				cursor_rect.x -= surface.get_font()->get_width(c.c_str());
		}
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

void TextBox::move_cursor_right()
{
	if (cursor_pos < surface.length())
	{
		// Advance the cursor 1 further in 
		// the string
		cursor_pos++;

		// Get the text for the current line 
		std::string line = surface.get_lines()[current_line];

		if (multiline)
		{
    		// Check if the user pressed the right arrow key
    		// and the cursor is at the end of the current
    		// line 
    		if (cursor_rect.x == bounding_rect.x + surface.get_font()->get_width(line.c_str()))
    		{
    			// Change the current line to the next one
    			current_line++;

    			cursor_pos = 0;

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
    			std::string c = "a";

    			// Add the width of that character to the cursor's x 
    			// value 
    			cursor_rect.x += surface.get_font()->get_width(c.c_str());
    		}
    	}
    	else
    	{
    		std::string c = "a";

    		if (cursor_pos == surface.get_visible_char_end() + 1)
    		{
    			surface.set_visible_char_end(cursor_pos);
    			surface.set_visible_char_start(surface.get_visible_char_start() + 1);
    			surface.reload(renderer);

    			cursor_rect.x = bounding_rect.x + bounding_rect.w;
    		}
    		else
				cursor_rect.x += surface.get_font()->get_width(c.c_str());
    	}

		// std::cout << "cursor_pos in right: " << cursor_pos << std::endl;
	}
}

void TextBox::move_cursor_up()
{
	// Move the cursor up a line 
	if (cursor_rect.y - surface.get_font()->get_line_height() >= bounding_rect.y)
		cursor_rect.y -= surface.get_font()->get_line_height();

	current_line--;

	if (current_line == surface.get_visible_start() - 1)
	{
		surface.set_visible_start(current_line);
		surface.set_visible_end(surface.get_visible_end() - 1);
		surface.reload(renderer);
	}

	// Get the current line
	std::string line = surface.get_lines()[current_line];

	if (cursor_pos > line.length())
	{
		cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(line.c_str());
		cursor_pos = line.length();
	}
}

void TextBox::move_cursor_down()
{
	if (current_line < surface.get_lines().size() - 1)
	{
		current_line++;

		if (current_line == surface.get_visible_end() + 1)
		{
			surface.set_visible_start(surface.get_visible_start() + 1);
			surface.set_visible_end(current_line);
			surface.reload(renderer);
		}

		if (surface.get_visible_start() == 0)
    		cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
    	else
    		cursor_rect.y = bounding_rect.y + (current_line - surface.get_visible_start()) * surface.get_font()->get_line_height();

		// if (cursor_rect.y + surface.get_font()->get_line_height()
		// 	< bounding_rect.y + bounding_rect.h)
		// 	cursor_rect.y += surface.get_font()->get_line_height();
		
		std::string line = surface.get_lines()[current_line];

		if (cursor_pos > line.length())
		{
			cursor_pos = line.length();
			cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(line.c_str());
		}
	}
	else
	{
		std::string line = surface.get_lines()[current_line];
		cursor_pos = surface.get_lines()[current_line].length();
		cursor_rect.x = bounding_rect.x + surface.get_font()->get_width(line.c_str());
	}
}

void TextBox::delete_char()
{
	// lop off character
    cursor_pos--;
    // std::cout << "cursor pos: " << cursor_pos << std::endl;
    surface.remove_char_at(renderer, cursor_pos, current_line);

    std::string c = "a";

	if (multiline)
    	cursor_rect.x = bounding_rect.x + cursor_pos * surface.get_font()->get_width(c.c_str());
    else
   		cursor_rect.x = bounding_rect.x + (cursor_pos - surface.get_visible_char_start()) * surface.get_font()->get_width(c.c_str());

   	if (surface.get_visible_start() == 0)
    	cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
}

void TextBox::insert_char(char c)
{
	// std::cout << "cursor_pos before insert(): " << cursor_pos << std::endl;
	        	// std::cout << "current_line before insert(): " << current_line << std::endl;

	// Append character at the cursor position
	const char * ch = "a";
	std::string str = "";
	str += c;

	surface.insert_text_at_index(renderer, str, cursor_pos, current_line);
	            
	cursor_pos++;

	//std::cout << "width: " << cursor_pos * surface.get_font()->get_width(ch) << std::endl;

	if (multiline)
		cursor_rect.x = bounding_rect.x + cursor_pos * surface.get_font()->get_width(ch);
	else 
	{
		if (cursor_pos > surface.get_max_chars_per_line())
			cursor_rect.x = bounding_rect.x + (cursor_pos - surface.get_visible_char_start()) * surface.get_font()->get_width(ch);
		else
	  	{
			cursor_rect.x = bounding_rect.x + cursor_pos * surface.get_font()->get_width(ch);
	  	}
	}

	if (surface.get_visible_start() == 0)
		cursor_rect.y = bounding_rect.y + current_line * surface.get_font()->get_line_height();
	else
		cursor_rect.y = bounding_rect.y + (current_line - surface.get_visible_start()) * surface.get_font()->get_line_height();
}

void TextBox::to_next_line()
{
	// Add a new line to the TextSurface
	surface.add_newline(renderer, cursor_pos, current_line);

	// Set the cursor to the TextBox's far-left
	// bound and add the length of a line to the
	// cursor's y 
	cursor_rect.x = bounding_rect.x; 

	if (cursor_rect.y + surface.get_font()->get_line_height()
		<= bounding_rect.y + bounding_rect.h)
		cursor_rect.y += surface.get_font()->get_line_height();
	
	current_line++;
	cursor_pos = 0;
}

void TextBox::handle_input(SDL_Event *e)
{
	// Handle focus 
	Control::handle_input(e);

	if (focus)
	{
		if (e->type == SDL_KEYDOWN)
		{
			if (e->key.keysym.sym == SDLK_BACKSPACE && surface.length() > 0)
	        {
	        	if ((current_line == 0 && cursor_pos != 0) || current_line != 0)
	        	{
	        		if (send)
	        		{
	        			std::stringstream stream;
	        			stream << "delete" << '\n'
	        			       << current_line << '\n'
	        				   << cursor_pos << '\n';

	        			parent->modify_textbox(stream.str());
	        		}

	            	delete_char();
	        	}
	        }

	        if (e->key.keysym.sym == SDLK_RETURN && multiline)
	        {
	        	if (send)
	        	{
	        		std::stringstream str;
	        		str << "enter" << '\n'
	        			<< current_line << '\n'
	        			<< cursor_pos << '\n';

	        		parent->modify_textbox(str.str());
	        	}

	        	to_next_line();
	        }

	        if (e->key.keysym.sym == SDLK_UP && current_line > 0)
	        {
	        	move_cursor_up();
	        }

	        if (e->key.keysym.sym == SDLK_DOWN)
	        {
	        	move_cursor_down();
	        }

	        if (e->key.keysym.sym == SDLK_LEFT)
	        {
        		move_cursor_left();
	        }

	        if (e->key.keysym.sym == SDLK_RIGHT)
	        {
	        	move_cursor_right();
	        }
		}
		// Check if the user entered text
	    else if( e->type == SDL_TEXTINPUT )
	    {
	        // Not copy or pasting
	        if( !( ( e->text.text[0] == 'c' || e->text.text[0] == 'C' ) && ( e->text.text[0] == 'v' || e->text.text[0] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
	        {
	        	if (send)
	        	{
	        		std::stringstream stream;
	        		stream << "insert" << '\n';
	        		stream << current_line << '\n'
	        			   << cursor_pos << '\n'
	        		   	   << (char)e->text.text[0] << '\n';

	        		parent->modify_textbox(stream.str());
	        	}

	        	insert_char(e->text.text[0]);
	        }
	    }
	}
}
