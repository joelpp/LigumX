#include <ctime>
#include <iomanip>
#include <sstream>
#include "StringUtils.h"
#include "FileUtils.h"
#include "LXError.h"

#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr


#pragma warning(disable : 4996) // TODO : deprecated std::localtime

std::string StringUtils::FromFile(const char* fileName)
{
	std::stringstream stream;

	std::fstream refFile = FileUtils::OpenFile(fileName);

	std::string line;
	while (std::getline(refFile, line))
	{
		stream << line;
		stream << '\n';
	}

	FileUtils::CloseFile(refFile);

	return stream.str();
}


std::vector<std::string> StringUtils::ReadFileLines(std::fstream& fileStream)
{
	std::vector<std::string> values;
	if (fileStream.is_open())
	{
		std::string line;
		while (std::getline(fileStream, line))
		{
			values.push_back(line);
		}
		fileStream.close();
	}

	return values;
}

std::vector<std::string> StringUtils::ReadFileLines(const char* fileName)
{
	std::fstream fileStream(fileName, std::fstream::in);
	return ReadFileLines(fileStream);
}


std::vector<std::string> StringUtils::ReadFileLines(const std::string& fileName)
{
	return ReadFileLines(fileName.c_str());
}


bool StringUtils::StringContains(const std::string& baseString, const std::string& searchedString)
{
	return baseString.find(searchedString) != std::string::npos;
}

bool StringUtils::StringContains(const std::string& s, char val)
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

std::vector<std::string> StringUtils::SplitString(const char* txt, char ch)
{
	return SplitString(LXString(txt), ch);
}

std::vector<std::string> StringUtils::SplitString(const std::string& txt, char ch)
{
	std::vector<std::string> strs;
	unsigned int pos = (unsigned int) txt.find(ch);
	if (pos > txt.length())
	{
		strs.push_back(txt);
		return strs;
	}

	unsigned int initialPos = 0;

	// Decompose statement
	while (pos != std::string::npos) {
		if (pos > txt.length())
		{
			break;
		}
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = (int) txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, (unsigned int)txt.size()) - initialPos + 1));

	return strs;
}


constexpr unsigned int StringUtils::Str2int(const char* str, int h)
{
	return !str[h] ? 5381 : (Str2int(str, h + 1) * 33) ^ str[h];
}

std::string StringUtils::RemoveSubstrings(const std::string& baseString, const std::string& substring)
{
	std::string cleanString = baseString;

	std::string::size_type n = substring.length();

	for (std::string::size_type i = cleanString.find(substring); i != std::string::npos; i = cleanString.find(substring))
	{
		cleanString.erase(i, n);
	}

	return cleanString;
}

std::string StringUtils::RemoveSubstrings(const std::string& baseString, const StringList& substrings)
{
	std::string cleanString = baseString;

	for (const std::string& substring : substrings)
	{
		cleanString = RemoveSubstrings(cleanString, substring);
	}

	return cleanString;
}

std::string StringUtils::GetSubstringBetweenCharacters(const std::string& str, char c0, char c1)
{
	unsigned int first = (unsigned int) str.find(c0);
	unsigned int last = (unsigned int) str.find(c1);
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

int StringUtils::Count(const std::string& s, const char c)
{
	return (int) std::count(s.begin(), s.end(), c);
}

float StringUtils::ToFloat(const std::string& str)
{
	return (float)std::atof(str.c_str());
}

int StringUtils::ToInt(const std::string& str)
{
	return std::atoi(str.c_str());
}

lxInt64 StringUtils::ToInt64(const std::string& str)
{
	return std::atoll(str.c_str());
}



bool StringUtils::IsCapitalLetter(const char& c)
{
	return !(c >= 'a' && c <= 'z');
}

bool StringUtils::IsLowerCaseLetter(const char& c)
{
	return !IsCapitalLetter(c);
}

std::string StringUtils::SeparateByCapitalLetters(const std::string& str)
{
	std::string toReturn = str;

	
	for (int charIndex = 0; charIndex < toReturn.size(); ++charIndex)
	{
		if (charIndex > 0 && IsCapitalLetter(toReturn[charIndex]) && !IsCapitalLetter(toReturn[charIndex - 1]))
		{
			toReturn.insert(charIndex, " ");
			charIndex++;
		}
	}

	return toReturn;
}


std::string StringUtils::Format(const std::string fmt_str, ...) 
{
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) 
	{
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

std::string StringUtils::ToStringPrecise(const float a_value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}
