
#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class File
{
private:
	
	// File stream, used for file operations 
	std::fstream stream;

	// The path to the file
	std::string path;

	// Flags that will be passed to the stream
	// (i.e. read/write permission)
	int mode;

public:

	// Initialize the mode to read and write
	// (The '|' operator shifts the bits of the number)
	File(std::string p) : path(p), mode(std::ios::out | std::ios::in )
	{
		// Open the file
		open();
	}

	// Close the file when the object is destoyed
	~File() { stream.close(); }

	// Open a file (with option to change the file
	// and mode that the object already contains)
	void open(std::string p = "", int flags = -1)
	{
		// If no changes were made to the object
		if (p != "")
		{
			path = p;

			if (flags != -1)
				mode = flags;
		}

		// If the file is open, close it
		stream.close();

		// Open the file with the mode flags 
		stream.open(path, mode);
	}

	// Clsoe the file if it's open
	void close() { if (stream.is_open() stream.close(); }

	// Get a vector of 'lines' from the text file.
	// Each line is designeted by a delimitting character
	// that is passed into the function (default is 
	// new lines)
	std::vector< std::string > get_lines(char del='\n')
	{
		// Create the vector for the lines
		std::vector< std::string > lines; 

		// Create a placeholder to hold each line
		std::string line = "";

		// While the stream can read lines from this file,
		// put the line in the second parameter of 
		// getline() with a line being all the characters
		// before and after the delimitting character
		while (std::getline(stream, line, del))
		{	
			lines.push_back(line);
		}

		return lines;
	}

};

#endif 
