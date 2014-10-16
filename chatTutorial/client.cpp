// SDL_net Client | r3dux.org | 14/01/2011

// Includes for non-blocking keyboard input
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h> // If we do not include termios.h the client WILL compile but it WILL NOT WORK!
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>

#include <SDL2/SDL_net.h>

using namespace std;

const unsigned short PORT        = 1234; // The port we are connecting to
const unsigned short BUFFER_SIZE = 512;  // Size of our message buffer (i.e. maximum length of characters in a message)

struct termios orig_termios;

// Function to reset the terminal to blocking mode
void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

// Sets the terminal mode to conio mode
void set_conio_terminal_mode()
{
    struct termios new_termios;

    // Take two copies - one for now, one for later
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    // register cleanup handler, and set the new terminal mode
    //atexit(reset_terminal_mode); // Commented out because I switch and swap terminal modes a lot - just remember to call reset_terminal_mode() when we finish up
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

// Fuction to check if a key has been pressed
int kbHit()
{
    // How long to wait for input
    // Note: As soon as we get input the wait is immediately over - so it's not like our typing rate is limited in any way!
    long waitSeconds      = 1L;
    long waitMicroSeconds = 0L;
    struct timeval tv = { waitSeconds, waitMicroSeconds };

    // Create a file descriptor set
    fd_set fds;

    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

// Function to read the contents of the keypress
int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        //cout << "About to return a number..." << endl;
        return r;
    }
    else
    {
        //cout << "About to return a character..." << endl;
        return c;
    }
}

int main(int argc, char **argv)
{
    const char *host;         // Where we store the host name

    IPaddress serverIP;       // The IP we will connect to
    TCPsocket clientSocket;   // The socket to use
    string    serverName;     // The server name

    string userInput = "";    // A string to hold our user input
    int inputLength  = 0;     // The length of our string in characters
    char buffer[BUFFER_SIZE]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.

    // Initialise SDL_net
    if (SDLNet_Init() < 0)
    {
        cout << "Failed to intialise SDN_net: " << /*SDLNet_GetError() <<*/ "\n";
        exit(-1); // Quit!
    }

    // Ask the user for a server to connect to - can be entered as a hostname (i.e. localhost etc.) or an IP address (i.e. 127.0.0.1 etc.)
    cout << "Server Name: ";
    //getline(cin, serverName); // Uncomment this and remove the below line to change the server we're connecting to...
    serverName = "localhost";

    // Create the socket set with enough space to store our desired number of connections (i.e. sockets)
    SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
    if (socketSet == NULL)
    {
        cout << "Failed to allocate the socket set: " << /*SDLNet_GetError() <<*/ "\n";
        exit(-1); // Quit!
    }
    else
    {
        cout << "Successfully allocated socket set." << endl;
    }

    // Try to resolve the host. If successful, this places the connection details in the serverIP object
    int hostResolved = SDLNet_ResolveHost(&serverIP, serverName.c_str(), PORT);

    if (hostResolved == -1)
    {
        cout << "Failed to resolve the server hostname: " << /*SDLNet_GetError() <<*/ "\nContinuing...\n";
    }
    else // If we successfully resolved the host then output the details
    {
        // Get our IP address in proper dot-quad format by breaking up the 32-bit unsigned host address and splitting it into an array of four 8-bit unsigned numbers...
        Uint8 * dotQuad = (Uint8*)&serverIP.host;

        //... and then outputting them cast to integers. Then read the last 16 bits of the serverIP object to get the port number
        cout << "Successfully resolved host to IP: " << (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1] << "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3];
        cout << " port " << SDLNet_Read16(&serverIP.port) << endl << endl;
    }

    // Try to resolve the IP of the server, just for kicks
    if ((host = SDLNet_ResolveIP(&serverIP)) == NULL)
    {
        cout << "Failed to resolve the server IP address: " << /*SDLNet_GetError() <<*/ endl;
    }
    else
    {
        cout << "Successfully resolved IP to host: " << host << endl;
    }

    // Flag to keep track of when to disconnect and finish up. We initially set it so that we CANNOT connect, and only change this to false when we got an "OK" response from the server
    bool shutdownClient = true;

    // Try to open a connection to the server and quit out if we can't connect
    clientSocket = SDLNet_TCP_Open(&serverIP);
    if (!clientSocket)
    {
        cout << "Failed to open socket to server: " << /*SDLNet_GetError() <<*/ "\n";
        exit(-1);
    }
    else // If we successfully opened a connection then check for the server response to our connection
    {
        cout << "Connection okay, about to read connection status from the server..." << endl;

        // Add our socket to the socket set for polling
        SDLNet_TCP_AddSocket(socketSet, clientSocket);

        // Wait for up to five seconds for a response from the server
        // Note: If we don't check the socket set and WAIT for the response, we'll be checking before the server can respond, and it'll look as if the server sent us nothing back
        int activeSockets = SDLNet_CheckSockets(socketSet, 5000);

        cout << "There are " << activeSockets << " socket(s) with data on them at the moment." << endl;

        // Check if we got a response from the server
        int gotServerResponse = SDLNet_SocketReady(clientSocket);

        if (gotServerResponse != 0)
        {
            cout << "Got a response from the server... " << endl;
            int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);

            cout << "Got the following from server: " << buffer << "(" << serverResponseByteCount << " bytes)" << endl;

            // We got an okay from the server, so we can join!
            if ( strcmp(buffer, "OK") == 0 )
            {
                // So set the flag to say we're not quitting out just yet
                shutdownClient = false;

                cout << "Joining server now..." << endl << endl;
            }
            else
            {
                cout << "Server is full... Terminating connection." << endl;
            }
        }
        else
        {
            cout << "No response from server..." << endl;
        }

    } // End of if we managed to open a connection to the server condition

    bool wrotePrompt = false; // Whether or not we've already written the prompt
    bool sendMessage = false; // Whether or not it's time to send the message (flips to true when the user presses return)

    // While it's not time to shutdown the client...
    while (shutdownClient == false)
    {
        // Write the prompt only once per line of input. This gets reset so that it's displayed again after a message is sent
        if (wrotePrompt == false)
        {
            cout << "Write something:" << endl;
            wrotePrompt = true;
        }

        // If we've detected that the user has pressed a key..
        set_conio_terminal_mode();
        int status = kbHit();
        reset_terminal_mode();

        //cout << "status is: " << status << endl;

        if (status != 0)
        {
            //cout << "key was pressed and status is" << status << endl;

            // Get the keypress
            set_conio_terminal_mode();
            char theChar = getch();
            reset_terminal_mode();

            // Output the character to stdout
            cout << theChar;

            // Flush the character to the screen
            fflush(stdout);

            // If the keypressed wasn't return then add the character to our message string
            if ((int)theChar != 13)
            {
                //cout << "Got the character: " << theChar << " (which is number: " << int(theChar) << ")" << endl;

                // Add the character to our input string
                userInput += theChar;
            }
            else // Otherwise (if the user pressed enter) then send the message
            {
                //cout << "user pressed return" << endl;

                // Copy our user's string into our char array called "buffer"
                strcpy( buffer, userInput.c_str() );

                // Calculate the length of our input and then add 1 (for the terminating character) to get the total number of characters we need to send
                inputLength = strlen(buffer) + 1;

                // Send the message to the server
                if (SDLNet_TCP_Send(clientSocket, (void *)buffer, inputLength) < inputLength)
                {
                    cout << "Failed to send message: " << /*SDLNet_GetError() <<*/ endl;
                    exit(-1);
                }
                else
                {
                    //cout << "Message sent successfully." << endl;

                    // If we've asked the server to shutdown or we want out then set the flag appropriately
                    if (sendMessage == true && (userInput == "quit" || userInput == "exit" || userInput == "shutdown"))
                    {
                        shutdownClient = true;
                    }

                    // Reset for the next message
                    cout << endl;
                    wrotePrompt = false;
                    sendMessage = false;
                    userInput = "";
                }

            } // End of message sending section

        } // End of if the user pressed a key test

        // Check our socket set for activity. Don't wait if there's nothing on the socket just continue
        int socketActive = SDLNet_CheckSockets(socketSet, 0);

        //cout << "Sockets with data on them at the moment: " << activeSockets << endl;

        if (socketActive != 0)
        {
            // Check if we got a response from the server
            int messageFromServer = SDLNet_SocketReady(clientSocket);

            if (messageFromServer != 0)
            {
                //cout << "Got a response from the server... " << endl;
                int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);

                cout << "Received: " << buffer << endl;// "(" << serverResponseByteCount << " bytes)" << endl;

                if (strcmp(buffer, "shutdown") == 0)
                {
                    cout << "Server is going down. Disconnecting..." << endl;
                    shutdownClient = true;
                }
            }
            else
            {
                //cout << "No response from server..." << endl;
            }

        } // End of if socket has activity check

    } // End of main while loop

    // Close our socket, cleanup SDL_net, reset the terminal mode and finish!
    SDLNet_TCP_Close(clientSocket);

    SDLNet_Quit();

    reset_terminal_mode();

    return 0;
}