// Names: Travis Banks, Austin Frandsen, and Ryan Frappier
// a simple client for networking with SDL_Net
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>

int main(int argc,char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();
	
	IPaddress ip;
	//write "127.0.0.1",1234 to connect to the server.cpp on your local machine
	SDLNet_ResolveHost(&ip,"127.0.0.1",1234);

	const char* http="GET / HTTP/1.1\nHost: www.linux.org\n\n";

	TCPsocket client=SDLNet_TCP_Open(&ip);

	SDLNet_TCP_Send(client,http,strlen(http)+1);

	char text[100];

	while(SDLNet_TCP_Recv(client,text,100))
		std::cout << text;
	
	SDLNet_TCP_Close(client);
	
	SDLNet_Quit();
	SDL_Quit();
}