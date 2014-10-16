
#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Canvas.h"
#include "LoginMenu.h"
#include "string.h"
#include <string>


const unsigned short PORT        = 1234; // The port we are connecting to
const unsigned short BUFFER_SIZE = 512;  // Size of our message buffer (i.e. maximum length of characters in a message)



class Program
{
private:
	int screen_width, screen_height;
	bool quit;

	SDL_Window   *window;
	SDL_Renderer *renderer;
    SDL_Event    e;

    LoginMenu login_menu;
    Canvas canvas;
    /*
    bool is_online;
    Network* net;
	*/
    const char *host;         // Where we store the host name

    IPaddress serverIP;       // The IP we will connect to
    TCPsocket clientSocket;   // The socket to use
    std::string serverName;     // The server name

    std::string userInput = "";    // A string to hold our user input
    int inputLength  = 0;     // The length of our string in characters
    char buffer[BUFFER_SIZE]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.

    
public:
	Program(int w, int h);

	~Program() { cleanup(); }

	// Calls init() and load_media()
	bool initialize_program();

	// Initializes SDL
	bool init();
	
	// loads all images, fonts, etc
	// needed for the program
	bool load_media();

	// The main loop of the program
	int  execute();

	// Quits SDL and all of the subsystems
	// used as well as deallocates memory
	void cleanup();

	// Network sends
	void send_message(std::string str);
};

#endif 
