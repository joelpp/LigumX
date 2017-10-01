#pragma once


#include "Common.h"

typedef std::vector<std::string> StringList;

std::vector<std::string> readFileLines(const char* fileName)
{
	std::vector<std::string> values;

	std::fstream refFile(fileName, std::fstream::in);
	std::string line;
	while (std::getline(refFile, line))
	{
		values.push_back(line);
	}
	refFile.close();

	return values;
}

bool stringContains(std::string s, char val)
{
	return s.find(val) != std::string::npos;
}

bool stringArrayContains(std::vector<std::string>& s, std::string& val)
{
	for (std::string& str : s)
	{
		if (val == str)
		{
			return true;
		}
	}
	return false;
}

std::vector<std::string> splitString(const std::string &txt, char ch)
{
	std::vector<std::string> strs;
	unsigned int pos = txt.find(ch);
	unsigned int initialPos = 0;

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, (std::min)(pos, txt.size()) - initialPos + 1));

	return strs;
}


constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

std::string RemoveSubstrings(std::string& baseString, std::string substring)
{
	std::regex pattern(substring);

	return std::regex_replace(baseString, pattern, "");
}

std::string RemoveSubstrings(std::string& baseString, StringList& substrings)
{
	std::string cleanString = baseString;
	
	for (std::string& substring : substrings)
	{
		cleanString = RemoveSubstrings(cleanString, substring);
	}

	return cleanString;
}

void TestSubstringRemover()
{
	std::string str("\tHello what is your name");

	StringList list;
	list.push_back("\t");
	list.push_back("h");


	std::cout << RemoveSubstrings(str, list);
}

std::string getSubstringBetweenCharacters(const std::string& str, char c0, char c1)
{
	unsigned first = str.find(c0);
	unsigned last = str.find(c1);
	return str.substr(first + 1, last - first - 1);
}