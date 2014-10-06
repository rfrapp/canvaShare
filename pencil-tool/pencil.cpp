#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include "SDL2_gfxPrimitives.h"
struct pos
{
    int x,y;
};

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

int screen_width = 1040, screen_height = 768;

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
    std::vector< std::vector< pos > > strokes;
    std::vector< pos > tmp;

    int x, y;
    bool press = false;
    // Movement and Buttons

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

            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&x,&y);
                press = true;
                pos p = {x,y};
                tmp.clear();
                tmp.push_back(p);
            }
            else if(e.type == SDL_MOUSEMOTION && press == true)
            {
                SDL_GetMouseState(&x,&y);
                pos p = {x,y};
                tmp.push_back(p);
            }
            else if(e.type == SDL_MOUSEBUTTONUP)
            {
                press = false;
                strokes.push_back(tmp);
                //tmp.clear();
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear the screen by drawing over the previous
        // frame with the RenderClearColor 
        SDL_RenderClear(renderer);

        SDL_GetMouseState(&x,&y);

        // Draw code goes here
        for(int i = 0; i < strokes.size(); i++)
        {
            for(int j = 0; j < strokes[i].size(); j++)
            {
                filledCircleRGBA(renderer, strokes[i][j].x, 
                           strokes[i][j].y, 1, 0, 0, 0, 255);
            }
        }

        for(int i = 0; i < tmp.size(); i++)
        {
            filledCircleRGBA(renderer, tmp[i].x, 
                           tmp[i].y, 1, 0, 0, 0, 255);
        }
        //canvas.draw();

        // Update the screen with the newly drawn content
        SDL_RenderPresent(renderer);
    }

	// Start code here

	

	return 0;
}
