// Name: Travis Banks, Austin Frandsen, and Ryan Frappier
// File: main.cpp
//
// Description: 
// This program shows color modulation in SDL 2

#include <iostream>
#include "Program.h"
#include <fstream>
#include "File.h"
#include <string>
#include <vector>

int main()
{
    // Create a program object with width 1024
    // and height 768
    // Program program(1024, 768);

    // Execute the main loop
    // return program.execute();

	File f("blah.txt");

	std::vector< std::string > v = f.get_lines();

	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << std::endl;
	}

    return 0;
}
