
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
	int max_lines;
	int visible_line_start, visible_line_end;
	int visible_char_start, visible_char_end;
	bool font_created;
	bool infinite;
	bool multiline;
	Font *font;
	std::vector< std::string > lines;
	SDL_Color text_color;

public:
	TextSurface(std::string font_path, int fs, int maxw, int maxh, 
		        bool multi_line=true, int r=0, int g=0, int b=0)
	 : font_size(fs), x(0), y(0), max_w(maxw), max_h(maxh),
	   max_chars_per_line(0), font_created(true), multiline(true),
	   max_lines(0), visible_line_start(0), visible_line_end(0),
	   visible_char_start(0), visible_char_end(0)
	{
		font = new Font(font_path.c_str(), font_size);

		text_color.r = r;
		text_color.g = g;
		text_color.b = b;

		lines.push_back("");

		int _w, _h;
		TTF_SizeText(font->get_ttf(), "a", &_w, &_h);
		max_chars_per_line = (max_w / _w) - 1;
		max_lines = (max_h / _h) - 1;
	}

	TextSurface(Font *f, int maxw, int maxh, int _x=0, int _y=0, bool multi_line=true, bool infinite_width=false, int r=0, int g=0, int b=0) 
	           : font(f), x(_x), y(_y), max_w(maxw), max_h(maxh),
	             max_chars_per_line(0), font_created(false), visible_line_start(0), visible_line_end(0),
				 multiline(multi_line),
	   			 visible_char_start(0), visible_char_end(0), infinite(infinite_width)
	{
		text_color.r = r;
		text_color.g = g;
		text_color.b = b;
		lines.push_back("");

		int _w, _h;
		TTF_SizeText(font->get_ttf(), "a", &_w, &_h);
		max_chars_per_line = (max_w / _w) - 1;
		max_lines = (max_h / _h) - 1;	
	}

	~TextSurface() { if (font_created) font->destroy(); }

	void reload(SDL_Renderer *r) { load_from_rendered_text(r); }

	void render(SDL_Renderer *renderer)
	{
		if (lines.size() > 0)
			Texture::render(renderer, x, y);
	}

	void render(SDL_Renderer *renderer, int _x, int _y)
	{
		if (lines.size() > 0)
		{
			Texture::render(renderer, _x, _y, NULL);
		}
	}

	void set_text(SDL_Renderer * r, std::string str)
	{
		if (!multiline)
		{
			if (lines.size() == 0)
				lines.push_back(str);
			else
				lines[0] = str;

			visible_char_start = 0;
			visible_char_end = str.length();
		}

		std::cout << "line: " << lines[0] << std::endl;

		load_from_rendered_text(r);
	}

	void insert_text_at_index(SDL_Renderer *r, std::string str, int & index, int & current_line)
	{
		std::string line = lines[current_line];
		int new_index = 0; 
		bool added_line = false;

		line = line.insert(index, str);
		lines[current_line] = line;

		if (multiline)
		{
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

					// If a word was found and needs
					// to be wrapped
					if (last_word_start != -1)
					{
						// get the word
						std::string substr = lines[j].substr(last_word_start + 1, len);
						
						// erase the word from the line
						lines[j] = lines[j].erase(last_word_start, diff);

						// If inserting there is line after 
						// the current one
						if (j == lines.size() - 1)
						{
							// Insert a new line containing
							// that wrapped word
							lines.push_back(substr);

							// move the cursor to the substring
							// length - 1
							new_index = substr.length() - 1;
						}
						else // There is a line after the current one
						{
							// Add the wrapped word to that
							// line 
							lines[j + 1].insert(0, substr);
							new_index = substr.length() - 1;
						}
					}
					else // No word to be wrapped
					{
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

					// A new line was created. so
					// increase the current line by 1
					// and change the cursor position to
					// the new one 
					if (j == current_line)
					{
						current_line++;
						index = new_index;
						added_line = true;
					}
				}
			}

			if (added_line)
			{
				visible_line_end = lines.size() - 1;

				if (visible_line_end > max_lines)
					visible_line_start++;
			}
		}
		else
		{
			visible_char_end++;

			if (lines[0].length() > max_chars_per_line)
			{
				visible_char_start++;
			}
		}

		// Recreate the surface for the
		// text 
		load_from_rendered_text(r);
	}

	void remove_char_at(SDL_Renderer *r, int & index, int & current_line)
	{
		std::string line = lines[current_line];

		// std::cout << "index: " << index << std::endl;
		// std::cout << "line: " << current_line << std::endl;

		if (index >= 0)
		{
			std::cout << "line before: " << line << std::endl;
			line = line.erase(index, 1);
			lines[current_line] = line;
			std::cout << "line after: " << line << std::endl;

			if (!multiline)
			{
				std::string s = "";

				for (int i = visible_char_start; i < visible_char_end; i++)
					s += line[i];

				std::cout << visible_char_start << ' ' << visible_char_end << ": " << s << std::endl;

				// if (visible_char_end > 0)
				// 	visible_char_end--;

				if (visible_char_start > 0)
					visible_char_start--;

				std::cout << visible_char_start << ' ' << visible_char_end << ": " << s << std::endl;
			}
		}
		else // The user hit delete on the beginning of a line
		{
			if (current_line - 1 >= 0)
			{
				if (lines[current_line - 1].length() 
					+ lines[current_line].length()
					< max_chars_per_line)
				{
					std::cout << "blah" << std::endl;
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
				else if (index == -1)
				{
					std::cout << "I am here!!!" << std::endl;
					lines.pop_back();
					current_line--;
					index = lines[current_line].length();
				}
			}
		}

		load_from_rendered_text(r);	
	}

	void add_newline(SDL_Renderer *r, int index, int line) 
	{ 
		lines.push_back(""); 

		if (index != lines[line].size())
		{
			std::string str = "";

			for (int i = index; i < lines[line].size(); i++)
			{
				str += lines[line][i];
			}

			lines[line].erase(index, str.length());

			lines[lines.size() - 1] = str;
		}

		visible_line_end++;
	
		if (visible_line_end > max_lines)
			visible_line_start++;

		load_from_rendered_text(r);
	}

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

	void set_x(int _x) { x = _x; }
	void set_y(int _y) { y = _y; }
	void set_visible_start(int s) { visible_line_start = s; }
	void set_visible_end(int s) { visible_line_end = s; }
	void set_visible_char_start(int s) { visible_char_start = s; }
	void set_visible_char_end(int s) { visible_char_end = s; }

	int get_max_lines() const { return max_lines; }
	int get_max_chars_per_line() const { return max_chars_per_line; }
	int get_visible_start() const { return visible_line_start; }
	int get_visible_end() const { return visible_line_end; }
	int get_visible_char_start() const { return visible_char_start; }
	int get_visible_char_end() const { return visible_char_end; }

	bool load_from_rendered_text(SDL_Renderer * r);

};

#endif
