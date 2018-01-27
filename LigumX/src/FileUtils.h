#pragma once
#include <fstream>

namespace FileUtils
{
	std::fstream OpenFile(const char* fileName);
	void CloseFile(std::fstream& file);

}