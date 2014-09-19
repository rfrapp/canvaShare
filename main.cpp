// Name: Travis Banks, Austin Frandsen, and Ryan Frappier
// File: main.cpp
//
// Description: 
// This program shows color modulation in SDL 2

#include <iostream>
#include "Program.h"
#include "File.h"
#include <fstream>

int main()
{
    // Create a program object with width 1024
    // and height 768
    //Program program(1024, 768);

    // Execute the main loop
    //return program.execute();
	File f("blah.txt");

	f.get_lines('|');

    return 0;
}