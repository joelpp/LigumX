#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <regex>
#include "LXSystem.h"

constexpr char gc_NewLine = '\n';

typedef std::vector<std::string> StringList;

namespace StringUtils
{
	std::string FromFile(const char* fileName);

	std::vector<std::string> ReadFileLines(std::fstream& fileStream);
	std::vector<std::string> ReadFileLines(const char* fileName);
	std::vector<std::string> ReadFileLines(const std::string& fileName);

	bool StringContains(const std::string& baseString, const std::string& searchedString);
	bool StringContains(const std::string& s, char val);

	bool StringArrayContains(std::vector<std::string>& s, std::string& val);

	std::vector<std::string> SplitString(const std::string &txt, char ch);

	constexpr unsigned int Str2int(const char* str, int h = 0);

	std::string RemoveSubstrings(const std::string& baseString, const std::string& substring);

	std::string RemoveSubstrings(const std::string& baseString, const StringList& substrings);

	std::string GetSubstringBetweenCharacters(const std::string& str, char c0, char c1);

	std::string GetTimestamp();

	bool DumpToFile(std::string& path, std::string& data);

	int Count(const std::string& s, const char c);

	float ToFloat(const std::string& str);

	int ToInt(const std::string& str);
	lxInt64 ToInt64(const std::string& str);

	bool IsCapitalLetter(const char&);
	bool IsLowerCaseLetter(const char& c);

	std::string SeparateByCapitalLetters(const std::string& str);

	std::string Format(const std::string fmt_str, ...);

	std::string ToStringPrecise(const float a_value, const int n = 6);

}

template<typename ... Args>
std::string lxFormat(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}