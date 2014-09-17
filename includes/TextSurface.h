
#ifndef TEXTSURFACE_H
#define TEXTSURFACE_H

#include "Texture.h"
#include <string>
#include <SDL2/SDL_ttf.h>
#include "Font.h"

class TextSurface : public Texture 
{
private:
	int x, y;
	int w, h, font_size;
	int max_w, max_h;
	int max_chars_per_line;
	bool font_created;
	Font *font;
	std::vector< std::string > lines;
	SDL_Color text_color;

public:
	TextSurface(std::string font_path, int fs, int maxw, int maxh, int r=0, int g=0, int b=0)
	 : font_size(fs), x(0), y(0), max_w(maxw), max_h(maxh),
	   max_chars_per_line(0), font_created(true)
	{
		font = new Font(font_path.c_str(), font_size);

		text_color.r = r;
		text_color.g = g;
		text_color.b = b;

		lines.push_back("");

		int _w, _h;
		TTF_SizeText(font->get_ttf(), "a", &_w, &_h);
		max_chars_per_line = (max_w / _w) - 1;
	}

	TextSurface(Font *f, int maxw, int maxh, int _x=0, int _y=0, int r=0, int g=0, int b=0) 
	           : font(f), x(_x), y(_y), max_w(maxw), max_h(maxh),
	             max_chars_per_line(0), font_created(false)
	{
		text_color.r = r;
		text_color.g = g;
		text_color.b = b;
		lines.push_back("");

		int _w, _h;
		TTF_SizeText(font->get_ttf(), "a", &_w, &_h);
		max_chars_per_line = (max_w / _w) - 1;
	}

	~TextSurface() { if (font_created) font->destroy(); }

	void render(SDL_Renderer *renderer)
	{
		if (lines.size() > 0)
			Texture::render(renderer, x, y);
	}

	void set_text(SDL_Renderer * r, std::string str)
	{
		load_from_rendered_text(r);
	}

	void insert_text_at_index(SDL_Renderer *r, std::string str, int index)
	{
		// std::cout << "index: " << index << std::endl;

		if (index >= length())
			append_to_text(r, str);
		else
		{
			int new_index = index;
			int edited_index = get_line_for_index(index);
			std::string line = lines[edited_index];

			// remove stuff from left
			for (int i = 0; i < edited_index; i++)
				new_index -= lines[i].length();

			// remove stuff from right
			for (int i = lines.size() - 1; i > edited_index; i--)
			{
				new_index -= lines[i].length();
			}

			line = line.insert(new_index, str);
			lines[edited_index] = line;

			// Check if any of the lines exceeded the 
			// width 
			// get last word start and end of edited line
			for (int j = edited_index; j < lines.size(); j++)
			{
				if (lines[j].length() > max_chars_per_line)
				{
					int last_word_start = -1;
					for (int i = lines[j].length(); i >= 0; i--)
					{
						if (lines[j][i] == ' ')
							last_word_start = i;
					}
					
					// The length of the edited line
					int len = lines[edited_index].length();
					int diff = len - last_word_start;

					if (last_word_start != -1)
					{
						std::string substr = lines[j].substr(last_word_start + 1, len);
						lines[j].erase(last_word_start, diff);

						if (j == lines.size() - 1)
							lines.push_back(substr + str);
						else
						{
							lines[j + 1].insert(0, substr + str);
						}
					}
					else
					{
						if (j == lines.size() - 1)
							lines.push_back(str);
						else
							lines[j + 1].insert(0, str);
					}
				}
			}

			load_from_rendered_text(r);
		}
	}

	void append_to_text(SDL_Renderer * r, std::string str)
	{
		int last_word_start = -1;

		if (lines[lines.size() - 1].length() > max_chars_per_line)
		{
			// get last word start and end
			for (int i = lines[lines.size() - 1].length(); i >= 0; i--)
			{
				if (lines[lines.size() - 1][i] == ' ')
					last_word_start = i;
			}
			
			// The length of the last line
			int len = lines[lines.size() - 1].length();
			int diff = len - last_word_start;
			
			// Wrap words 
			if (last_word_start != -1)
			{
				std::string substr = lines[lines.size() - 1].substr(last_word_start + 1, len);
				lines[lines.size() - 1].erase(last_word_start, diff);
				lines.push_back(substr + str);
			}
			else
			{
				lines.push_back(str);
			}
		}
		else
			lines[lines.size() - 1] += str;

		load_from_rendered_text(r);
	}

	void remove_char_at(SDL_Renderer *r, int index)
	{
		if (index == length() - 1)
			pop(r);
		else
		{
			int new_index = index;
			int edited_index = get_line_for_index(index);
			std::string line = lines[edited_index];

			// remove stuff from left
			for (int i = 0; i < edited_index; i++)
				new_index -= lines[i].length();

			// remove stuff from right
			for (int i = lines.size() - 1; i > edited_index; i--)
			{
				new_index -= lines[i].length();
			}
			std::cout << "new_index: " << new_index << std::endl;

			if (new_index == -1 && edited_index > 0)
			{
				edited_index--;
				new_index = lines[edited_index].length() - 1;
				line = lines[edited_index];
			}

			if (line.length() > 1)
				line = line.erase(new_index, 1);
			else
				line = "";

			lines[edited_index] = line;

			// Check if the edited line is an empty
			// string. If so, remove it
			if (lines[edited_index] == "")
				lines.pop_back();

			load_from_rendered_text(r);
		}
	}

	void pop(SDL_Renderer * r)
	{
		int len = lines[lines.size() - 1].length();

		if (len != 0)
			lines[lines.size() - 1].erase(len - 1, 1);
		else
			lines.pop_back();


		load_from_rendered_text(r);
	}

	void add_newline() { lines.push_back(""); }

	std::string text() const 
	{ 
		std::string lines_text = "";
		for (int i = 0; i < lines.size() - 1; i++)
		{
			lines_text += lines[i];
		}
		lines_text += lines[lines.size() - 1];

		return lines_text;
	}
	SDL_Color color()                       const { return text_color;      }
	int       length()                      const { return text().length(); }
	int       get_width()                   const { return width;           }
	int       get_x()                       const { return x;               }
	int       get_y()                       const { return y;               }
	int       num_lines()                   const { return lines.size();    }
	Font *    get_font()                    const { return font;            }
	std::vector < std::string > get_lines() const { return lines;           }

	std::string get_char_at_index(int index)
	{
		int c = 0;
		std::string str = "";

		for (int i = 0; i < lines.size(); i++)
		{
			for (int j = 0; j < lines[i].length(); j++)
			{
				if (c == index)
				{
					str = lines[i][j];
					return str;
				}
				c++;
			}
		}

		return str;
	}

	int get_line_for_index(int index)
	{
		int found_index = -1;
		int c = 0;

		for (int i = 0; i < lines.size(); i++)
		{
			for (int j = 0; j < lines[i].length(); j++)
			{
				if (c == index)
				{
					found_index = i;
					break;
				}
				c++;
			}
		}

		if (index == length())
			found_index = lines.size() - 1;

		return found_index;
	}

	void set_x(int _x) { x = _x; }
	void set_y(int _y) { y = _y; }

	bool load_from_rendered_text(SDL_Renderer * r);

};

#endif
