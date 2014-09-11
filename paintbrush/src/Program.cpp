
#include "Program.h"

bool Program::initialize_program()
{
	if (!init())
		return false;
	if (!load_media())
		return false;

	return true; 
}

bool Program::init()
{
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
                canvas.set_renderer(renderer);

                canvas.init_controls();
            }
        }
    }

    return success;
}

bool Program::load_media()
{
	bool success = true;
    
    // load fonts, imgs, etc here

    return success;
}

int Program::execute()
{
	// Initialize SDL and load media
    if (!initialize_program())
    	return 1;

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

            canvas.handle_input(&e);
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear the screen by drawing over the previous
        // frame with the RenderClearColor 
        SDL_RenderClear(renderer);

        // Draw code goes here
        canvas.draw();

        // Update the screen with the newly drawn content
        SDL_RenderPresent(renderer);
    }

    return 0;
}

void Program::cleanup()
{
	SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    renderer = NULL;
    window   = NULL;

    IMG_Quit();
    SDL_Quit();
}
