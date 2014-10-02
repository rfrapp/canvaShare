// Name: Travis Banks, Austin Frandsen, and Ryan Frappier
// File: main.cpp
//
// Description: 
// This program shows color modulation in SDL 2

#include <iostream>
#include "Program.h"
#include <fstream>
//#include "File.h"
#include <string>
#include <vector>

int main()
{
    // Create a program object with width 1024
    // and height 768
    Program program(1024, 768);

    // Execute the main loop
    return program.execute();

	// File f("blah.txt");

	// std::vector< std::string > v = f.read_lines();

	// std::cout << "Lines: " << std::endl;
	// for (int i = 0; i < v.size(); i++)
	// {
	// 	std::cout << v[i] << std::endl;
	// }

	// std::cout << "Lines delimited: " << std::endl;
	// std::vector< std::vector< std::string > > v2;
	// v2 = f.get_lines_delimited('|');

	// for (int i = 0; i < v2.size(); i++)
	// {
	// 	std::cout << "- Line " << i << std::endl;
	// 	for (int j = 0; j < v2[i].size(); j++)
	// 		std::cout << v2[i][j] << " ";
	// 	std::cout << std::endl;
	// }

 //    return 0;
}
