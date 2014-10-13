#ifndef NETWORK_H
#define NETWORK_H


#include <SDL2/SDL_net>
#include <iostream>
#include <cstring>
#include "Program.h"
#include "Canvas.h"


class network
{
private:
    SDLNet_SocketSet server;
    TCPsocket connection;

    char tmp[1400];
public:
    network(const char* ip);
    ~network();
    void send_canvas_item(std::vector)
};




#endif
