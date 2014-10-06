
#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Font
{
private:
	TTF_Font *font;
	std::string path;
	int size, style, line_height;

public:
	Font(TTF_Font *f) : font(f)
	{
	}

	Font(std::string p, int s, int st=TTF_STYLE_NORMAL)
	: path(p), size(s), style(st), font(NULL)
	{
		reload();
	}

	void reload()
	{
		if (font != NULL)
		{
			TTF_CloseFont(font);
			font = NULL;
		}

		font = TTF_OpenFont(path.c_str(), size);
		TTF_SetFontStyle(font, style);
		line_height = TTF_FontLineSkip(font);
	}

	TTF_Font * get_ttf() const { return font; }

	int get_width(const char *c) const 
	{
		int w; 
		TTF_SizeText(font, c, &w, NULL);
		return w;
	}

	int get_line_height() const 
	{
		return line_height;
	}

	int get_height(char c) const 
	{
		int h; 
		TTF_SizeText(font, &c, NULL, &h);
		return h;
	}

	int get_style() const { return style; }

	void set_size(int s)  { size = s; }
	void set_style(int s) { style = s; }

	~Font()
	{
		destroy();
	}

	void destroy() { TTF_CloseFont(font); font = NULL; }
};

#endif
