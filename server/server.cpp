//a server for networking with SDL_net
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>
#include <vector>

int main(int argc,char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	IPaddress ip;
	SDLNet_ResolveHost(&ip,NULL,1234);
	
	TCPsocket server=SDLNet_TCP_Open(&ip);
	TCPsocket client;

	const char* text="Hello!\nYou are now connected to canvaShare!\n";

    char canvas[100];
    
    while(1)
	{
		client=SDLNet_TCP_Accept(server);
		if(client)
		{
			std::cout << "client connected" << std::endl;
			//here you can communitcate with the client
			SDLNet_TCP_Send(client,text,strlen(text)+1);
			//SDLNet_TCP_Close(client);
            SDLNet_TCP_Recv(client, canvas,strlen(canvas)+1);
			break;
		}
	}
    SDLNet_TCP_Close(client);
	SDLNet_TCP_Close(server);
	
	SDLNet_Quit();
	SDL_Quit();
}
