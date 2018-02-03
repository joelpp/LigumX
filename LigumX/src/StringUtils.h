#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <regex>

typedef std::vector<std::string> StringList;

namespace StringUtils
{
	std::string FromFile(const char* fileName);

	std::vector<std::string> ReadFileLines(const char* fileName);

	bool StringContains(std::string s, char val);

	bool StringArrayContains(std::vector<std::string>& s, std::string& val);

	std::vector<std::string> SplitString(const std::string &txt, char ch);

	constexpr unsigned int Str2int(const char* str, int h = 0);

	std::string RemoveSubstrings(const std::string& baseString, const std::string& substring);

	std::string RemoveSubstrings(const std::string& baseString, const StringList& substrings);

	std::string GetSubstringBetweenCharacters(const std::string& str, char c0, char c1);

	std::string GetTimestamp();

	bool DumpToFile(std::string& path, std::string& data);

	int Count(const std::string& s, const char c);

	int ToInt(const std::string& str);

	bool IsCapitalLetter(const char&);
	bool IsLowerCaseLetter(const char& c);

	std::string SeparateByCapitalLetters(const std::string& str);


}
