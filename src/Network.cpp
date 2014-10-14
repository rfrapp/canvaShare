#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "Network.h"


Network::Network(const char* ipchar)
{
    SDLNet_Init();
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, ipchar, 1234) == -1)
    {
        std::cout << "There was an error\n";
    }
    
    connection = SDLNet_TCP_Open(&ip);
    if (connection == NULL)
    {
        std::cout << "Bad IP error\n";
    }

    server = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(server, connection);
}

/*
Network::~Network()
{
    SDLNet_TCP_Send(connection, "2 \n", 4);
    SDLNet_TCP_Close();
    SDLNetFreeSocketSet(server);
    SDLNet_Quit();
}
*/
