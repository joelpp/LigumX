#pragma once
#include <fstream>
#include "StringUtils.h"

namespace FileUtils
{
	std::vector<LXString> GetAllFilesInDirectory(const char* dirName);

	std::fstream OpenFile(const char* fileName);
	void CloseFile(std::fstream& file);

	int PathContains(StringList& pathElements, StringList& tokens);
	LXString BuildPathFromTokens(StringList& tokens, int startIndex);
}