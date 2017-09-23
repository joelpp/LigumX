#pragma once

#include "LXClass.h"
class CodeRegion
{
public:
	CodeRegion(LXClass& aClass)
		: m_Class(aClass)
	{

	}

	void WriteToFile()
	{
		std::string headerFilePath = g_GenerationRootDir + m_Class.m_Name + m_FileExtension;
		std::vector<std::string> lines = readFileLines(headerFilePath.c_str());

		// saerch for code_region line
		int index = 0;
		int classBeginIndex = 0;

		for (std::string& line : lines)
		{
			TokenList tokens = splitString(line, ' ');

			if (IsBeginMarker(tokens))
			{
				classBeginIndex = index;
			}
			if (line == GetBeginString())
			{
				SetBeginIndex(index);
			}
			if (line == GetEndString())
			{
				SetEndIndex(index);
			}

			index++;
		}

		int beginIndex = GetEndIndex() == 0 ?
			classBeginIndex + 2 :
			GetBeginIndex();

		index = 0;
		std::fstream headerFile(headerFilePath.c_str(), std::ios::out);

		if (lines.size() == 0)
		{

			headerFile << GetFinalOutput();
			headerFile << std::endl;
		}

		for (std::string& line : lines)
		{
			if (index < beginIndex || index > GetEndIndex())
			{
				headerFile << line;
				headerFile << std::endl;
			}

			index++;

			if ((bWriteCodeRegions && index == beginIndex) || beginIndex == 0)
			{
				headerFile << GetFinalOutput();
				headerFile << std::endl;
			}
		}

		headerFile.close();
	}

	std::string GetFinalOutput()
	{
		return m_Stream.str();
	}

	std::string GetBeginString()
	{
		return m_CodeRegionBeginString;
	}

	std::string GetEndString()
	{
		return m_CodeRegionEndString;
	}

	void WriteLine(std::string line)
	{
		m_Stream << line;
		m_Stream << std::endl;
	}

	int GetBeginIndex() { return m_BeginIndex; }
	void SetBeginIndex(int val) { m_BeginIndex = val; }
	int GetEndIndex() { return m_EndIndex; }
	void SetEndIndex(int val) { m_EndIndex = val; }

	LXClass& GetClass()
	{
		return m_Class;
	}

	virtual void Process() = 0;
	virtual bool IsBeginMarker(TokenList& token) { return false; }


protected:
	std::stringstream m_Stream;
	std::string m_IDString;
	std::string m_FileExtension;
	std::string m_CodeRegionBeginString;
	std::string m_CodeRegionEndString;
	LXClass m_Class;

	int m_BeginIndex = 0;
	int m_EndIndex = 0;
};