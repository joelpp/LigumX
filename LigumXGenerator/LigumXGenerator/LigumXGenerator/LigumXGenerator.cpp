// LigumXGenerator.cpp : Defines the entry point for the console application.
//
#define NOMINMAX
#include <windows.h>
#include "stdafx.h"
#include <string>
#include <iostream>

#include <iostream>
#include <algorithm>
#include <string>
#include <windows.h>
#include <vector>
#include <fstream>
#include <cassert>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <sstream>
#include <unordered_map>

#define NAMESPACE_BEGIN namespace LigumXGenerator {
#define NAMESPACE_END }


typedef std::vector<std::string> TokenList;

const std::string g_CodeRegionBeginString = "#pragma region ";
const std::string g_CodeRegionEndString = "#pragma endregion ";
std::string g_GenerationRootDir;
bool bWriteCodeRegions = true;

std::unordered_map<std::type_index, int> g_TypeMap;

struct Variable
{
	bool IsAPointer()
	{
		return m_IsPtr;
	}

	std::string m_Name;
	std::string m_Type;
	bool m_IsPtr;
};

typedef std::vector<Variable> VariableList;

enum FileType
{
	FileType_Source,
	FileType_Header,
	FileType_Generator,
	FileType_Invalid,
	FileType_NumFileTypes
};

struct LXClass
{
	std::string m_Name;
	std::string m_FileNames[FileType_NumFileTypes];
	VariableList m_Members;
};
typedef std::vector<LXClass> ClassList;

struct GeneratorFile
{
	std::string m_Name;
	std::vector<LXClass> m_Contained;
};

enum CodeRegionType
{
	CodeRegionType_ClassForwardDecl,
	CodeRegionType_ClassHeader,
	CodeRegionType_ClassSource,
	CodeRegionType_NumTypes
};


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

std::vector<std::string> split(const std::string &txt, char ch)
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


bool DiscardTokens(TokenList& tokens)
{
	return  (tokens[0] == "{") ||
		(tokens[0] == "") ||
		(tokens.size() == 0);
}

bool IsClassDeclaration(TokenList& tokens)
{
	return tokens[0] == "class";
}

bool IsIncludeDeclaration(TokenList& tokens)
{
	return tokens[0] == "#include";
}

bool EndClassDeclaration(TokenList& tokens)
{
	return tokens[0] == "}";
}

bool IsMethodDeclaration(TokenList& tokens)
{
	bool isMethod = false;

	for (std::string& token : tokens)
	{
		if (std::find(token.begin(), token.end(), '(') != token.end())
		{
			isMethod = true;
			break;
		}
	}

	return isMethod;
}

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
			TokenList tokens = split(line, ' ');

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
		for (std::string& line : lines)
		{
			if (index < beginIndex || index > GetEndIndex())
			{
				headerFile << line;
				headerFile << std::endl;
			}

			index++;

			if (bWriteCodeRegions && index == beginIndex)
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
	int GetEndIndex() { return m_EndIndex;  }
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

class ForwardDeclarationRegion : public CodeRegion
{
	public:
	ForwardDeclarationRegion(LXClass& aClass)
		: CodeRegion(aClass)
	{
		m_IDString = "FORWARD_DECLARATIONS";
		m_FileExtension = ".h";
		m_CodeRegionBeginString = g_CodeRegionBeginString + " " + m_IDString + " " + m_Class.m_Name;
		m_CodeRegionEndString = g_CodeRegionEndString + " " + m_IDString + " " + m_Class.m_Name;
	}

	bool IsBeginMarker(TokenList& token) { return IsIncludeDeclaration(token); }

	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;

		std::vector<std::string> classesToDeclare;
		WriteLine("#include \"property.h\"");
		for (Variable v : m_Class.m_Members)
		{
			if (v.IsAPointer())
			{
				if(!stringArrayContains(classesToDeclare, v.m_Type))
				{
					classesToDeclare.push_back(v.m_Type);
				}
			}
		}

		for (std::string& str : classesToDeclare)
		{
			WriteLine("class " + str + ";");
		}
	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}


	void Process()
	{
		// Write class header region
		WriteHeader();

		WriteFooter();

		WriteToFile();
	}
};


class ClassSourceRegion : public CodeRegion
{
public:
	ClassSourceRegion(LXClass& aClass)
		: CodeRegion(aClass)
	{
		m_IDString = "CLASS_SOURCE";
		m_FileExtension = ".cpp";
		m_CodeRegionBeginString = g_CodeRegionBeginString + " " + m_IDString + " " + m_Class.m_Name;
		m_CodeRegionEndString = g_CodeRegionEndString + " " + m_IDString + " " + m_Class.m_Name;
	}

	bool IsBeginMarker(TokenList& token) { return IsIncludeDeclaration(token); }

	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;
	}

	void WriteBody()
	{
		std::string propertyCountVarName = "g_" + m_Class.m_Name + "PropertyCount";
		int numProperties = m_Class.m_Members.size();

		WriteLine("const ClassPropertyData " + m_Class.m_Name + "::g_" + m_Class.m_Name + "Properties[] = ");
		WriteLine("{");

		for (int i = 0; i < numProperties; ++i)
		{
			std::string&  varName = m_Class.m_Members[i].m_Name;
			WriteLine("{ \"" + varName + "\", offsetof(" + m_Class.m_Name + ", m_" + varName + "), " + (m_Class.m_Members[i].m_Type == "\tglm::vec3" ? "1" : "0") + " },");
		}

		WriteLine("};");
	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}


	void Process()
	{
		// Write class header region
		WriteHeader();

		WriteBody();

		WriteFooter();

		WriteToFile();
	}
};

class ClassHeaderRegion : public CodeRegion
{
public:
	ClassHeaderRegion(LXClass& aClass)
		: CodeRegion(aClass)
	{
		m_IDString = "HEADER";
		m_FileExtension = ".h";
		m_CodeRegionBeginString = g_CodeRegionBeginString + " " + m_IDString + " " + m_Class.m_Name;
		m_CodeRegionEndString = g_CodeRegionEndString	  + " " + m_IDString + " " + m_Class.m_Name;
	}
	bool IsBeginMarker(TokenList& token) { return IsClassDeclaration(token); }


	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;
		m_Stream << "static const int ClassID = " << std::hash_value(m_Class.m_Name) << ";";
		m_Stream << std::endl;
	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}

	void WriteClassVariableGettersAndSetters()
	{
		m_Stream << "public:";
		m_Stream << std::endl;
		for (Variable var : m_Class.m_Members)
		{
			// Write getter
			bool ptr = var.IsAPointer();
			m_Stream << (ptr ? "" : "const");
			m_Stream << (var.m_Type);
			m_Stream << (ptr ? "*" : "&");
			m_Stream << (" ");
			m_Stream << "Get" << var.m_Name << "() { return m_" << var.m_Name << "; }; ";
			m_Stream << std::endl;

			// Write setter
			m_Stream << "void Set" << var.m_Name << "(" << var.m_Type;
			m_Stream << (ptr ? "*" : "");
			m_Stream <<	" value) { m_" << var.m_Name << " = value; };";
			m_Stream << std::endl;
		}
	}

	void WriteProperties()
	{
		WriteLine("public:");

		// TODO : functions to compute this or store it
		std::string propertyCountVarName = "g_" + m_Class.m_Name +"PropertyCount";
		int numProperties = m_Class.m_Members.size();
		WriteLine("static const int " + propertyCountVarName + " = " + std::to_string(numProperties) + ";");

		WriteLine("static const ClassPropertyData g_" + m_Class.m_Name + "Properties[" + propertyCountVarName + "];");

		m_Stream << std::endl;
	}

	void WriteBody()
	{
		WriteClassVariableGettersAndSetters();

		m_Stream << "private:" << std::endl;
		for (Variable var : m_Class.m_Members)
		{
			m_Stream << var.m_Type;
			m_Stream << (var.IsAPointer() ? "*" : "");
			m_Stream << " m_" << var.m_Name << ";";
			m_Stream << std::endl;
		}

		WriteProperties();

	}

	void Process()
	{
		// Write class header region
		WriteHeader();

		WriteBody();


		WriteFooter();

		WriteToFile();
	}
};

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

FileType GetTypeFromHeader(std::string fileType)
{
	switch (str2int(fileType.c_str()))
	{
	case str2int("h"):
	{
		return FileType_Header;
		break;
	}
	case str2int("cpp"):
	{
		return FileType_Source;
		break;
	}
	case str2int("gen"):
	{
		return FileType_Generator;
		break;
	}
	default:
		return FileType_Invalid;
		break;
	}
}

ClassList createLXClass(std::vector<std::string>& lines)
{
	ClassList classes;
	bool generatingClass = false;

	LXClass currentClass;
	{
		// read lines
		for (std::string& line : lines)
		{
			// tokenize
			TokenList tokens = split(line, ' ');

			if (DiscardTokens(tokens))
			{
				continue;
			}
			if (IsMethodDeclaration(tokens))
			{
				// todo implement
				continue;
			}

			if (IsClassDeclaration(tokens))
			{
				currentClass.m_Name = tokens[1];
				generatingClass = true;
				continue;
			}

			if (EndClassDeclaration(tokens))
			{
				generatingClass = false;
				classes.push_back(currentClass);
				currentClass = LXClass();
				continue;
			}

			// then its a variable

			Variable variable;
			variable.m_Name = tokens[1];;
			variable.m_Type = tokens[0];

			variable.m_IsPtr = stringContains(variable.m_Type, '*');
			variable.m_Type.erase(std::remove(variable.m_Type.begin(), variable.m_Type.end(), '*'), variable.m_Type.end());

			currentClass.m_Members.push_back(variable);

			// not invalid or a class declaration. we're adding to the class desc
			void;
		}
	}

	return classes;
}

template<typename T>
void SetToZero(T* arr, int numElements)
{
	memset(arr, 0, numElements * sizeof(T));
}

void processSingleGeneratorFile(GeneratorFile& genFile)
{
	std::string filePath = g_GenerationRootDir + genFile.m_Name;
	std::vector<std::string> lines = readFileLines(filePath.c_str());

	//CodeRegion codeRegion;
	ClassList classes = createLXClass(lines);

	LXClass& theClass = classes[0];

	ClassHeaderRegion headerRegion(theClass);
	headerRegion.Process();

	ForwardDeclarationRegion forwardRegion(theClass);
	forwardRegion.Process();

	ClassSourceRegion sourceRegion(theClass);
	sourceRegion.Process();
}

void processGeneratorFiles(std::vector<GeneratorFile>& generatorFiles)
{
	for (GeneratorFile& genFile : generatorFiles)
	{
		processSingleGeneratorFile(genFile);
	}
}

int main()
{

	const unsigned len = GetCurrentDirectory(0, 0);
	if (!len)
		return 1;
	std::string currentDir(len, '\0');
	if (!GetCurrentDirectoryA(len, &currentDir[0]))
		return 1;

	currentDir = currentDir.substr(0, currentDir.size() - 1);

	std::string projectName = "LigumX";
	
	std::string generationOffset = "\\..\\..\\..\\";
	
	g_GenerationRootDir = currentDir + generationOffset + projectName + "\\src\\";
	std::replace(g_GenerationRootDir.begin(), g_GenerationRootDir.end(), '\\', '/');

	std::cout << "Current dir : " << currentDir << std::endl;
	std::cout << "Generation root dir : " << g_GenerationRootDir << std::endl;


	HANDLE hFind;
	WIN32_FIND_DATA data;


	std::vector<std::string> srcFileList;

	hFind = FindFirstFile("C:/Users/Joel/Documents/LigumX/LigumXGenerator/LigumXGenerator/LigumXGenerator/../../../LigumX/src/*"/*generationRootDir.c_str()*/, &data);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			srcFileList.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));


		FindClose(hFind);
	}


	std::vector<GeneratorFile> generatorFiles;
	for (std::string& file : srcFileList)
	{
		int dotIndex = file.find_first_of('.');

		std::string fileName = file.substr(0, dotIndex);
		std::string fileHeader = file.substr(dotIndex + 1, file.length());

		FileType type = GetTypeFromHeader(fileHeader);

		if (type == FileType_Invalid)
		{
			continue;
		} 
		else if (type == FileType_Generator)
		{
			GeneratorFile genFile;
			genFile.m_Name = fileName + ".gen";

			generatorFiles.push_back(genFile);
		}
		else if (type == FileType_Header)
		{

		}
		else if (type == FileType_Source)
		{

		}

	}

	processGeneratorFiles(generatorFiles);

	return 0;
}

