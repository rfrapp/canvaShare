
#include "TextSurface.h"

bool TextSurface::load_from_rendered_text(SDL_Renderer *r)
{
	// Destroy the previous surface
	destroy();

	std::string lines_text = "";
	// std::cout << "lines size: " << lines.size() << std::endl;

	if (multiline)
	{
		for (int i = visible_line_start; i <= visible_line_end; i++)
		{
			lines_text += lines[i];

			if (i != lines.size() - 1)
				lines_text += '\n';
		}
	}
	else
	{
		if (lines.size() > 0)
		{
			//std::cout << "lines[0]: " << lines[0] << std::endl;
			if (!is_password_box)
			{
				for (int i = visible_char_start; i <= visible_char_end; i++)
					lines_text += lines[0][i];
			}
			else
			{
				// THE PROCEDING LINE IS A HOTFIX.
				// TODO: FIGURE OUT WHAT'S GOING WRONG
				// WHEN IT'S A PASSWORD BOX 
				visible_char_end = lines[0].length() - 1;
				for (int i = visible_char_start; i <= visible_char_end; i++)
					lines_text += "*";	
			}
		}
	}

	SDL_Surface * text_surface = TTF_RenderText_Blended_Wrapped(font->get_ttf(), lines_text.c_str(), text_color, max_w);

	if (text_surface == NULL)
		return false;
	else
	{
		texture = SDL_CreateTextureFromSurface(r, text_surface);

		if (texture == NULL)
			return false;
		else
		{
			width = text_surface->w;
			height = text_surface->h;
			pitch = text_surface->pitch;
		}

		SDL_FreeSurface(text_surface);
		text_surface = NULL;
	}

	return true; 
}
