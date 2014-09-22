
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

	void insert_text_at_index(SDL_Renderer *r, std::string str, int & index, int & current_line)
	{
		std::string line = lines[current_line];
		int new_index = 0; 

		line = line.insert(index, str);
		lines[current_line] = line;

		// Check if any of the lines exceeded the 
		// width 
		// get last word start and end of edited line
		for (int j = current_line; j < lines.size(); j++)
		{
			if (lines[j].length() > max_chars_per_line)
			{
				int last_word_start = -1;
				for (int i = lines[j].length(); i >= 0; i--)
				{
					if (lines[j][i] == ' ')
					{
						last_word_start = i;
						break;
					}
				}
				
				// The length of the line
				int len = lines[j].length();
				int diff = len - last_word_start;

				if (last_word_start != -1)
				{
					std::string substr = lines[j].substr(last_word_start + 1, len);
					lines[j] = lines[j].erase(last_word_start, diff);

					if (j == lines.size() - 1)
					{
						lines.push_back(substr);
						new_index = substr.length() - 1;
					}
					else
					{
						lines[j + 1].insert(0, substr);
						new_index = lines[j + 1].length() + substr.length() - 1;
					}
				}
				else
				{
					std::cout << "j + 1: " << j + 1 
							  << "lines.size() - 1: "
							  << lines.size() - 1; 

					if (j == lines.size() - 1)
					{
						lines.push_back(str);
						new_index = str.length() - 1;
					}
					else
					{
						lines[j + 1].insert(0, str);
						new_index = lines[j + 1].length() + str.length();
					}
				}

				if (j == current_line)
				{
					current_line++;
					index = new_index;
				}

				std::cout << std::endl << std::endl;
				for (int k = 0; k < lines.size(); k++)
					std::cout << k << " :" << lines[k] << std::endl;
				std::cout << std::endl << std::endl;

			}
		}

		load_from_rendered_text(r);
	}

	void remove_char_at(SDL_Renderer *r, int & index, int & current_line)
	{
		std::string line = lines[current_line];

		std::cout << "index: " << index << std::endl;
		std::cout << "line: " << current_line << std::endl;

		if (index >= 0)
		{
			line = line.erase(index, 1);
			lines[current_line] = line;
		}
		else // The user hit delete on the beginning of a line
		{
			if (current_line - 1 >= 0)
			{
				if (lines[current_line - 1].length() 
					+ lines[current_line].length()
					< max_chars_per_line)
				{
					lines[current_line - 1] += lines[current_line];
					index = lines[current_line - 1].length() - lines[current_line].length();
					std::cout << "index after mod: " << index << std::endl;
					lines[current_line] = "";

					// shift every other line down 1
					for (int i = current_line + 1; i < lines.size(); i++)
						lines[i - 1] = lines[i];

					if (current_line == lines.size() - 1)
						lines.pop_back();

					current_line--;
				}
			}
		}

		load_from_rendered_text(r);	
	}

	void add_newline() { lines.push_back(""); }

	std::string text() const 
	{ 
		std::string lines_text = "";
		for (int i = 0; i < lines.size(); i++)
		{
			lines_text += lines[i];
		}

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
