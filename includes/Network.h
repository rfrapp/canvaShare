#ifndef NETWORK_H
#define NETWORK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
//#include "Program.h"
//#include "Canvas.h"
//#include "CanvasItem.h"
	

class Network
{
//private:
SDLNet_SocketSet server;
TCPsocket connection;

const char* tmp;

public:
    Network(const char* ipchar);
    ~Network();
    //void send_canvas_item(Canvas* c);
};


#endif
