#pragma once
#include <iostream>  
#include <fstream>  
#include <cstdlib>
#include <memory>
#include <sstream>


namespace fileops
{
	bool readTextFromFile(const char* name,std::string& buf)
	{
		std::ifstream fp;
		fp.open(name);
		if (!fp.is_open())
		{
			std::cout << "Error opening file" << std::endl;
			return false;
		}
		long start, end;
		fp.seekg(0, std::ios::beg);
		start = fp.tellg();
		fp.seekg(0, std::ios::end);
		end = fp.tellg();
		long size = end - start;
		fp.seekg(0, std::ios::beg);

		std::stringstream buffer;
		buffer << fp.rdbuf();

		buf = buffer.str();

		fp.close();
		return true;
	};
}