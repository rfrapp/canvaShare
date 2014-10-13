
#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class File
{
private:
	
	// File strea, used for file operations 
	std::fstream write_stream;

	std::fstream read_stream;

	// The path to the file
	std::string path;

	// Flags that will be passed to the stream
	// (i.e. read/write permission)
	int read_mode, write_mode;

	std::vector< std::string > lines;

public:

	// Initialize the mode to read and write
	// (The '|' operator shifts the bits of the number)
	File(std::string p) : path(p), read_mode(std::ios::in | std::ios::out), 
	                      write_mode(std::ios::out | std::ios::in)
	{
		// Open the file
		open();
	}

	// Close the file when the object is destoyed
	~File() { close(); }

	// Open a file (with option to change the file
	// and mode that the object already contains)
	void open(std::string p = "", int read_flags = -1, int write_flags = -1)
	{
		// If no changes were made to the object
		if (p != "")
		{
			path = p;

			if (read_flags != -1)
				read_mode = read_flags;

			if (write_flags != -1)
				write_mode = write_flags;
		}

		// If the file is open, close it
		close();

		// Open the file with the mode flags 
		read_stream.open(path.c_str(), std::ios::in | std::ios::out);

		write_stream.open(path.c_str(), std::ios::in | std::ios::out);
	}

	// Clsoe the file if it's open
	void close() 
	{ 
		if (read_stream.is_open()) 
			read_stream.close(); 

		if (write_stream.is_open())
			write_stream.close();
	}

	// Get a vector of 'lines' from the text file.
	// Each line is designeted by a delimitting character
	// that is passed into the function (default is 
	// new lines)
	std::vector< std::string > read_lines(char del='\n')
	{
		lines.clear();

		// Create a placeholder to hold each line
		std::string line = "";

		// While the stream can read lines from this file,
		// put the line in the second parameter of 
		// getline() with a line being all the characters
		// before and after the delimitting character
		while (getline(read_stream, line, del))
		{	
			lines.push_back(line);
		}

		return lines;
	}

	void write_delimited_to_file(std::vector< std::vector< std::string > > users)
	{
		for (int i = 0; i < users.size(); i++)
		{
			for (int j = 0; j < users[i].size(); j++)
			{
				write_stream << users[i][j] << " ";
			}
			write_stream << '\n';
		}
	}

	std::vector< std::vector< std::string > > get_lines_delimited(char del=' ')
	{
		std::vector< std::vector< std::string > > v;

		if (lines.size() == 0)
			read_lines();

		for (int i = 0; i < lines.size(); i++)
		{
			std::stringstream stream;
			std::string tmp = "";
			std::vector< std::string > t_v;

			stream << lines[i];

			while (getline(stream, tmp, del))
			{
				t_v.push_back(tmp);
			}

			if (t_v.size() > 0)
				v.push_back(t_v);
		}

		return v;
	}

	std::vector< std::string > get_lines() const { return lines; }

};

#endif 
