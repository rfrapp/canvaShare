
#include "TextSurface.h"

bool TextSurface::load_from_rendered_text(SDL_Renderer *r)
{
	// Destroy the previous surface
	destroy();

	std::string lines_text = "";
	// std::cout << "lines size: " << lines.size() << std::endl;

	for (int i = 0; i < lines.size(); i++)
	{
		lines_text += lines[i];

		if (i != lines.size() - 1)
			lines_text += '\n';
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
