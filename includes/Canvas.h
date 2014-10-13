
#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <sstream>

#include "Control.h"
#include "Button.h"
#include "TextBox.h"
#include "Menu.h"
#include "Texture.h"
#include "PaintBrushTool.h"
#include "RectangleTool.h"
#include "TriangleTool.h"
#include "CircleTool.h"
#include "UndoTool.h"
#include "RedoTool.h"
#include "ScreenshotTool.h"
#include "PreviousPageTool.h"
#include "NextPageTool.h"
#include "CanvasItem.h"
#include "ColorRect.h"
#include "EraserTool.h"
#include "TextSurface.h"
#include "PencilTool.h"

class Tool;

class Canvas : public Menu 
{
private:
	enum Tool_IDs
	{
		PAINT_BRUSH_ID = 0,
		PENCIL_ID,
		RECTANGLE_ID,
		TRIANGLE_ID,
		CIRCLE_ID,
		UNDO_ID,
		REDO_ID,
		SCREENSHOT_ID,
		PREVPAGE_ID,
		NEXTPAGE_ID,
		ERASE_ID
	};

	// The width and height of the canvas
	int w, h;
	SDL_Renderer *renderer;
	std::vector< Control* > controls;

	// The foreground color for drawing
	Uint32 foreground;
	int fg_r, fg_b, fg_g, fg_a;

	// The background color for filling 
	// shapes 
	Uint32 background; 
	int bg_r, bg_b, bg_g, bg_a;

	int brush_radius; 

	Font *font;

	std::string cursor;

	int current_tool_index;
	std::vector< Tool * > tools;
	Rect draw_bounds; 

	std::vector< CanvasItem > canvas_items;
	std::vector< CanvasItem > undone_items; 
	CanvasItem * copied_item;

	int current_page;
	std::vector< Texture > pages;

	Rect foreground_rect; 
	Rect background_rect;
	std::vector< ColorRect > color_rects;
	bool background_selected;
	bool quit;
	int selected_item;

	TextSurface * page_surface;

	Point * clicked_pos;
	Point * last_move_pos;

public:
	Canvas(SDL_Renderer *r, int width, int height) 
	      : w(width), h(height), renderer(r),
	        foreground(0x000000), background(0xFFFFFFFF),
	        cursor(""), current_tool_index(-1),
	        draw_bounds(10, 40, width - 50, height - 80),
	        brush_radius(1), fg_r(0), fg_g(0), fg_b(0), fg_a(255),
	        bg_r(0), bg_g(0), bg_b(0), bg_a(255), quit(false),
	        current_page(0), background_selected(true), page_surface(NULL),
	        selected_item(-1), clicked_pos(NULL), last_move_pos(NULL)
	{
	}

	~Canvas()
	{
		// Clear the memory which was allocated for
		// the controls
		for (int i = 0; i < controls.size(); i++)
			delete controls[i];

		for (int i = 0; i < tools.size(); i++)
			delete tools[i];

		// Clear the memory allocated for the font
		font->destroy();

		if (page_surface != NULL)
			delete page_surface;

		if (clicked_pos != NULL)
			delete clicked_pos;
	}

	// Creates the Control objects needed for the
	// canvas and adds them to controls
	void init_controls();

	// Draws controls and CanvasObjects to the 
	// screen
	void draw();
	void handle_input(SDL_Event *e);

	void get_notification(std::string event, int id);
	int get_brush_radius() const  { return brush_radius; }
	Uint8 get_foreground_r() const { return fg_r; }
	Uint8 get_foreground_g() const { return fg_g; }
	Uint8 get_foreground_b() const { return fg_b; }
	Uint8 get_foreground_a() const { return fg_a; }

	Uint8 get_background_r() const { return bg_r; }
	Uint8 get_background_g() const { return bg_g; }
	Uint8 get_background_b() const { return bg_b; }
	Uint8 get_background_a() const { return bg_a; }

	std::vector< CanvasItem > & get_items() { return canvas_items; }

	void set_brush_radius(int r)  { brush_radius = r; }
	void increment_brush_radius() { brush_radius++;   }
	void decrement_brush_radius() { brush_radius--;   }
	void set_cursor(std::string c);

	void add_page();

	void add_canvas_item(const CanvasItem & i);

	void undo_canvas_item();
	void redo_canvas_item();
	void page_left();
	void page_right();
	int get_page() const { return current_page; }

	// Loads fonts, images, etc.
	bool load_media();
	bool is_move_tool() const;

	// Setter for the renderer property 
	void set_renderer(SDL_Renderer *r) { renderer = r; }
	SDL_Renderer * get_renderer() const { return renderer; }

	bool has_quit() const { return quit; }
	void set_quit(bool q) { quit = q; }
	int get_width() const { return w; }
	int get_height() const { return h; }
};

#endif
