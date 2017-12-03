#include "StringUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

#pragma warning(disable : 4996) // deprecated std::localtime

std::vector<std::string> StringUtils::ReadFileLines(const char* fileName)
{
	std::vector<std::string> values;

	std::fstream refFile(fileName, std::fstream::in);
	if (refFile.is_open())
	{
		std::string line;
		while (std::getline(refFile, line))
		{
			values.push_back(line);
		}
		refFile.close();
	}

	return values;
}

bool StringUtils::StringContains(std::string s, char val)
{
	return s.find(val) != std::string::npos;
}

bool StringUtils::StringArrayContains(std::vector<std::string>& s, std::string& val)
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

std::vector<std::string> StringUtils::SplitString(const std::string &txt, char ch)
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
	strs.push_back(txt.substr(initialPos, std::min(pos, (unsigned int)txt.size()) - initialPos + 1));

	return strs;
}


constexpr unsigned int StringUtils::Str2int(const char* str, int h)
{
	return !str[h] ? 5381 : (Str2int(str, h + 1) * 33) ^ str[h];
}

std::string StringUtils::RemoveSubstrings(const std::string& baseString, std::string substring)
{
	//std::regex pattern(substring);

	//return std::regex_replace(baseString, pattern, "");

	std::string cleanString = baseString;

	std::string::size_type n = substring.length();

	for (std::string::size_type i = cleanString.find(substring); i != std::string::npos; i = cleanString.find(substring))
	{
		cleanString.erase(i, n);
	}

	return cleanString;
}

std::string StringUtils::RemoveSubstrings(const std::string& baseString, StringList& substrings)
{
	std::string cleanString = baseString;

	for (std::string& substring : substrings)
	{
		cleanString = RemoveSubstrings(cleanString, substring);
	}

	return cleanString;
}

std::string StringUtils::GetSubstringBetweenCharacters(const std::string& str, char c0, char c1)
{
	unsigned first = str.find(c0);
	unsigned last = str.find(c1);
	return str.substr(first + 1, last - first - 1);
}

std::string StringUtils::GetTimestamp()
{
	auto t = std::time(nullptr);	
	auto tm = std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(tm, "%d-%m-%Y %H-%M-%S");

	return oss.str();
}


bool StringUtils::DumpToFile(std::string& path, std::string& data)
{
	std::ofstream out(path);

	if (out.is_open())
	{
		out << data;
		out.close();

		return true;
	}

	return false;
}
