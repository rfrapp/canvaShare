#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "SDL2_gfxPrimitives.h"

struct Rect
{
    int x1, y1; 
    int x2, y2;
};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

std::vector< Rect > rects;
Rect current;
bool clicked = false;

int screen_width = 640, screen_height = 480;

void handle_input(SDL_Event *e);
void draw();

bool init()
{
    // bool
	bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        success = false;
    else
    {
    	// Create the window with title "canvaShare",
    	// at the default x and y position on the screen,
    	// with width: screen_width and height: screen_height,
    	// then give the window focus using SDL_WINDOW_SHOWN

        window = SDL_CreateWindow("canvaShare", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  screen_width, screen_height, SDL_WINDOW_SHOWN);

        // If the window was not created, 
        // then there was an error
        if (window == NULL)
            success = false;
        else
        {
        	// Create the renderer. This is what's used
        	// to draw things in SDL2.

            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            // Exit if there was an error creating the
            // renderer
            if (renderer == NULL)
                success = false;
            else
            {
            	// This sets the color that will be drawn at each frame
            	// before drawing everthing else (i.e. the 'fill' color)
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            
                // Set the canvas' renderer to the newly created
                // one 
                //canvas.set_renderer(renderer);

                //canvas.init_controls();
            }
        }
    }

    return success;
}


int main()
{ 
	init();
	// Initialize SDL and load media
   // if (!initialize_program())
    	//return 1;
	bool quit = false;
    // The main program loop
    while (!quit)
    {
    	// Check for events
        while (SDL_PollEvent(&e))
        {
        	// Check if the user clicked the 'X' button
        	// on the window. If so, then quit
        	// the program
            if (e.type == SDL_QUIT)
                quit = true;

            handle_input(&e);
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear the screen by drawing over the previous
        // frame with the RenderClearColor 
        SDL_RenderClear(renderer);

        draw();

        // Update the screen with the newly drawn content
        SDL_RenderPresent(renderer);
    }

	// Start code here

	

	return 0;
}

void handle_input(SDL_Event * e)
{
    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        current.x1 = x;
        current.y1 = y;
        current.x2 = x;
        current.y2 = x;
        clicked = true;
    }
    else if (e->type == SDL_MOUSEMOTION)
    {
        if (clicked)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);

            current.x2 = x;
            current.y2 = y;
        }
    }
    else if (e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        current.x2 = x;
        current.y2 = y;

        rects.push_back(current);

        current.x1 = 0;
        current.x2 = 0;
        current.y1 = 0;
        current.y2 = 0;
        clicked = false;
    }
}

void draw()
{
    filledCircleRGBA(renderer, 
               current.x1 + (current.x2 - current.x1) / 2,
               current.y1 + (current.y2 - current.y1) / 2, 
               (current.x2 - current.x1) / 2,
               0xFF, 0x00, 0x00, 0xFF);

    for (int i = 0; i < rects.size(); i++)
    {
        filledCircleRGBA(renderer, 
                   rects[i].x1 + (rects[i].x2 - rects[i].x1) / 2,
                   rects[i].y1 + (rects[i].y2 - rects[i].y1) / 2, 
                   (rects[i].x2 - rects[i].x1) / 2,
                   0xFF, 0x00, 0x00, 0xFF);
    }
}
