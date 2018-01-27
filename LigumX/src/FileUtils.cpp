#include "FileUtils.h"
#include "LXError.h"



std::fstream FileUtils::OpenFile(const char* fileName)
{
	std::fstream file(fileName, std::fstream::in);
	if (file.is_open())
	{
	}
	else
	{
		lxAssert0();
	}

	return file;
}

void FileUtils::CloseFile(std::fstream& file)
{
	if (file.is_open())
	{
		file.close();
	}
	else
	{
		lxAssert0();
	}
}
