// LigumXGenerator.cpp : Defines the entry point for the console application.
//
#define NOMINMAX
#include "stdafx.h"
#include <intrin.h>
#include <ctime>

#include "Common.h"
#include "LXClass.h"
#include "CodeRegion.h"
#include "ClassSourceRegion.h"
#include "ClassHeaderRegion.h"
#include "StringUtils.h"
#include "ForwardDeclarationRegion.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif



#define NAMESPACE_BEGIN namespace LigumXGenerator {
#define NAMESPACE_END 

enum FileSelectionMode
{
	Normal = 0,
	ForceAll,
	OnlyFirst,
	SelectFile,
	Count
};

int g_Now;

struct GeneratorFile
{
	std::string m_Name;
	std::vector<LXClass> m_Contained;
};

// todo : finish this
struct TypeFileEntry
{
	TypeFileEntry()
	{
	}

	TypeFileEntry(const std::string& string)
	{
		m_TypeName = string;
	}

	std::string m_TypeName;
};

struct LogFileEntry
{
	const char m_Separator = '|';

	LogFileEntry()
	{
	}

	LogFileEntry(const std::string& string)
	{
		std::vector<std::string> tokens = splitString(string, m_Separator);
		m_FileName = tokens[0];
		m_Time = std::atoi(tokens[1].c_str());
	}

	std::string ToString() const
	{
		return std::string(m_FileName + m_Separator + std::to_string(m_Time));
	}

	bool Process(const std::string& fileName, int timeLastModified)
	{
		if (timeLastModified > m_Time)
		{
			m_Time = timeLastModified;
			return true;
		}

		return false;
	}

	std::string m_FileName;
	int m_Time;
};

struct LogFile
{
	LogFile() {};

	LogFile(const std::string& fileName)
	{
		m_FileName = g_GenerationRootDir + fileName;

		std::vector<std::string> lines = readFileLines(m_FileName.c_str());

		for (const std::string& line : lines)
		{
			LogFileEntry entry(line);
			m_Entries.push_back(entry);
		}
	}

	bool ProcessFile(const std::string& fileName, int timeLastModified)
	{
		bool found = false;
		for (LogFileEntry& entry : m_Entries)
		{
			found = entry.m_FileName == fileName;

			if (found)
			{
				return entry.Process(fileName, timeLastModified);
			}
		}

		if (!found)
		{
			LogFileEntry newEntry;

			newEntry.m_FileName = fileName;
			newEntry.m_Time = timeLastModified;

			m_Entries.push_back(newEntry);

			return true;
		}

		return false;
	}

	void Save()
	{
		std::stringstream sstream;

		for (const LogFileEntry& entry : m_Entries)
		{
			std::string line = entry.ToString();

			sstream << line << std::endl;
		}

		std::fstream logFile(m_FileName, std::ios::out);
		if (logFile.is_open())
		{
			logFile << sstream.str();
		}
		logFile.close();
	}

	std::string m_FileName;
	std::vector<LogFileEntry> m_Entries;
};


std::string g_DebugClassNameBreak;

std::map<std::string, int> g_FoundTypes;
std::map<std::string, int> g_AdditionalEnums;

LogFile g_LogFile;



void PrintLine(const char * line)
{
	std::cout << line << std::endl;
}

void PrintEmptyLine()
{
	std::cout << std::endl;
}

void AddToTypesMap(std::map<std::string, int>& map, std::string type)
{
	StringList toRemoveFromFoundTypes = { "::" };
	std::string entryKey = RemoveSubstrings(type, toRemoveFromFoundTypes);
	map[entryKey] = 1;
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

#define ELSE_RETURN_DEFAULT(typeName, defaultValue) \
if (type == typeName) \
{ \
return defaultValue; \
} \

std::string DefaultValueForType(bool isAPointer, const std::string& type)
{
	if (isAPointer)
	{
		return "nullptr";
	}

	if (type == "")
	{

	}
	ELSE_RETURN_DEFAULT("bool", "false")
		ELSE_RETURN_DEFAULT("int", "0")
		ELSE_RETURN_DEFAULT("float", "0.f")
		ELSE_RETURN_DEFAULT("glm::ivec2", "glm::ivec2(0, 0)")
		ELSE_RETURN_DEFAULT("glm::vec2", "glm::vec2(0, 0)")
		ELSE_RETURN_DEFAULT("glm::vec3", "glm::vec3(0, 0, 0)")
	else
	{
		return "";
	}
}

ClassList createLXClass(std::vector<std::string>& lines)
{
	ClassList classes;
	bool generatingClass = false;
	bool generatingEnum = false;
	LXClass currentClass;
	LXEnum currentEnum;
	{
		Variable variable;
		int classPropertyFlags = 0;
		int varPropertyFlags = 0;

		// read lines

		for (std::string& line : lines)
		{
			bool isPropertyFlags = IsPropertyFlags(line);

			std::string sanitizedLine = RemoveSubstrings(line, g_UnwantedSubstrings);
			// tokenize
			TokenList tokens = splitString(sanitizedLine, ' ');

			if (DiscardTokens(tokens))
			{
				continue;
			}

			if (isPropertyFlags)
			{
				if (generatingClass)
				{
					varPropertyFlags = GetVarPropertyFlags(tokens, variable);
				}
				else
				{
					classPropertyFlags = GetClassPropertyFlags(tokens);
					currentClass.m_PropertyFlags = classPropertyFlags;
				}

				continue;
			}

			//if (IsMethodDeclaration(tokens))
			//{
			//	// todo implement
			//	continue;
			//}

			if (IsEnumDeclaration(tokens))
			{
				currentEnum.m_Name = tokens[1];
				generatingEnum = true;
				continue;
			}

			if (IsClassDeclaration(tokens))
			{
				currentClass.m_Name = tokens[1];
				generatingClass = true;

				int numTokensForInclude = 3;
				if (tokens.size() > 2)
				{
					currentClass.m_ParentName = tokens[tokens.size() - 1];
				}

				//Variable objectIDVar;
				//objectIDVar.m_IsPtr = false;
				//objectIDVar.m_IsTemplate = false;
				//objectIDVar.m_Name = "ObjectID";
				//objectIDVar.SetType("int");
				//objectIDVar.m_PropertyFlags = 0;
				//currentClass.m_Members.push_back(objectIDVar);

				//Variable nameVar;
				//nameVar.m_IsPtr = false;
				//nameVar.m_IsTemplate = false;
				//nameVar.m_Name = "Name";
				//nameVar.SetType("std::string");
				//nameVar.m_PropertyFlags = 0;
				//currentClass.m_Members.push_back(nameVar);


				//g_DebugClassNameBreak = "SectorTool";
				if (g_DebugClassNameBreak == currentClass.m_Name)
				{
					__debugbreak();
				}

				continue;
			}

			if (EndClassDeclaration(tokens))
			{
				if (generatingClass)
				{
					AddToTypesMap(g_FoundTypes, currentClass.m_Name);

					//for (Variable& var : currentClass.m_Members)
					//{
					//	if (var.m_PropertyFlags & PropertyFlags_Enum)
					//	{
					//		AddToTypesMap(g_AdditionalEnums, var.m_Type);
					//	}
					//}

					if (currentClass.m_ParentName.empty() && currentClass.m_Name != "LXObject")
					{
						currentClass.m_ParentName = "LXObject";
					}

					currentClass.m_IsValid = true;
					classes.push_back(currentClass);
					currentClass = LXClass();
				}
				else if (generatingEnum)
				{
					currentClass.m_Enums.push_back(currentEnum);
					currentEnum = LXEnum();
				}

				generatingClass = false;
				generatingEnum = false;
				continue;
			}

			// then its a variable

			if (generatingEnum)
			{
				currentEnum.m_Values.push_back(tokens[0]);

				std::string defaultValue = tokens.size() == 3 ? tokens[2] : "";
				currentEnum.m_DefaultValues.push_back(defaultValue);
			}
			else
			{
				for (LXEnum& anEnum : currentClass.m_Enums)
				{
					if (tokens[0] == anEnum.m_Name)
					{
						varPropertyFlags |= PropertyFlags_Enum;
						break;
					}
				}

				std::string& varType = tokens[0];
				variable.SetType(varType);
				variable.m_IsPtr = stringContains(varType, '*') && !stringContains(varType, "*>");

				variable.CheckForTemplate();
				
				RemoveSubstrings(varType, "*");
				variable.SetType(varType);
				variable.RemoveTemplateDeclaration();

				variable.m_Name = tokens[1];




				//variable.GetType().erase(std::remove(variable.GetType().begin(), variable.GetType().end(), '*'), variable.GetType().end());

				//AddToTypesMap(g_FoundTypes, variable.GetType());

				variable.m_PropertyFlags = varPropertyFlags;

				unsigned int sizeWithDefautValue = 4;
				unsigned int defaultValueTokenIndex = 3;
				variable.m_DefaultValue = tokens.size() == sizeWithDefautValue ?
					tokens[defaultValueTokenIndex] :
					"";

				if (variable.m_DefaultValue.empty())
				{
					variable.m_DefaultValue = DefaultValueForType(variable.IsAPointer(), variable.GetType());
				}

				currentClass.m_Members.push_back(variable);

				variable = Variable();

				// reset property flags
				varPropertyFlags = 0;
			}

		}

	}

	// if there was no class in the file
	if (classes.size() == 0)
	{
		// push it anyway
		classes.push_back(currentClass);
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

	if (theClass.m_Name.empty())
	{
		StringList tokens = splitString(genFile.m_Name, '.');
		theClass.m_Name = tokens[0];
	}

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

void SystemPause()
{
	system("pause");
}

#define EMPLACE_PROPERTY_FLAG(flag, name) \
g_PropertyFlagsNames.emplace(flag, #flag); \
g_PropertyFlagsStringMap.emplace(name, flag);

void InitializeGenerator()
{

	g_LogFile = LogFile("GenFileLog.log");
	g_Now = (int)std::time(nullptr);

	// Substrings we want to remove from property names
	g_UnwantedSubstrings.push_back("\t");
	g_UnwantedSubstrings.push_back(";");
	g_UnwantedSubstrings.push_back("[");
	g_UnwantedSubstrings.push_back("]");
	g_UnwantedSubstrings.push_back(",");
	g_UnwantedSubstrings.push_back("    ");

	// Type names that have a corresponding gui callback
	g_AllowedTypesForGUI.push_back("bool");
	g_AllowedTypesForGUI.push_back("glm::vec3");

	EMPLACE_PROPERTY_FLAG(PropertyFlags_Hidden, "hidden");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_ReadOnly, "readonly");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_SetCallback, "setcallback");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_GetCallback, "getcallback");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_MinValue, "min");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_MaxValue, "max");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Transient, "transient");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_NonEditable, "noneditable");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Adder, "adder");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Enum, "enum");

	g_ClassPropertyFlagsStringMap.emplace("postserialization", ClassPropertyFlags_PostSerialization);
	g_ClassPropertyFlagsStringMap.emplace("globalinstance", ClassPropertyFlags_GlobalInstance);

	g_LXTypeToImguiCallName.emplace("bool", "Bool");
	g_LXTypeToImguiCallName.emplace("int", "Int");
	g_LXTypeToImguiCallName.emplace("float", "Float");
	g_LXTypeToImguiCallName.emplace("long", "Long");
	g_LXTypeToImguiCallName.emplace("glm::ivec2", "IVec2");
	g_LXTypeToImguiCallName.emplace("glm::ivec3", "IVec3");
	g_LXTypeToImguiCallName.emplace("glm::ivec4", "IVec4");
	g_LXTypeToImguiCallName.emplace("glm::vec2", "Vec2");
	g_LXTypeToImguiCallName.emplace("glm::vec3", "Vec3");
	g_LXTypeToImguiCallName.emplace("glm::vec4", "Vec4");
	g_LXTypeToImguiCallName.emplace("std::string", "String");
	g_LXTypeToImguiCallName.emplace("std::vector", "ObjectPtr_Vector");

	g_DefaultMinForType.emplace("int", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("LONG", "LX_LIMITS_LONG_MIN");
	g_DefaultMinForType.emplace("glm::ivec2", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("glm::ivec3", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("glm::ivec4", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("float", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec2", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec3", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec4", "LX_LIMITS_FLOAT_MIN");

	g_DefaultMaxForType.emplace("int", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("LONG", "LX_LIMITS_LONG_MAX");
	g_DefaultMaxForType.emplace("glm::ivec2", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("glm::ivec3", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("glm::ivec4", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("float", "LX_LIMITS_FLOAT_MAX");
	g_DefaultMaxForType.emplace("glm::vec2", "LX_LIMITS_FLOAT_MAX");
	g_DefaultMaxForType.emplace("glm::vec3", "LX_LIMITS_FLOAT_MAX");
	g_DefaultMaxForType.emplace("glm::vec4", "LX_LIMITS_FLOAT_MAX");
}

void WriteLineToFile(std::fstream& file, const char* line)
{
	file << line << std::endl;
}

void WriteLineToFile(std::fstream& file, std::string& line)
{
	WriteLineToFile(file, line.c_str());
}

#if 0
void GeneratePropertyFile()
{
	std::string propertyFileName = "property.h";
	std::string filePath = g_GenerationRootDir + propertyFileName;

	std::fstream propertyFile(filePath, std::fstream::out);
	if (propertyFile.is_open())
	{
		WriteLineToFile(propertyFile, "#pragma once");
		WriteLineToFile(propertyFile, "#include <fstream>");
		WriteLineToFile(propertyFile, "// This file is auto generated. Any modification will be deleted on next LXGenerator run.");
		WriteLineToFile(propertyFile, "");

		// Types
		WriteLineToFile(propertyFile, "enum LXType");
		WriteLineToFile(propertyFile, "{");
		for (auto& it = g_FoundTypes.begin(); it != g_FoundTypes.end(); ++it)
		{
			std::string typeName = "LXType_";
			typeName += it->first;

			WriteLineToFile(propertyFile, "	" + typeName + ",");
		}

		//for (auto& it = g_AdditionalEnums.begin(); it != g_AdditionalEnums.end(); ++it)
		//{
		//	std::string typeName = "LXType_";
		//	typeName += it->first;

		//	WriteLineToFile(propertyFile, "	" + typeName + ",");
		//}


		auto it = g_FoundTypes.find("LXType_char");
		if (it == g_FoundTypes.end())
		{
			WriteLineToFile(propertyFile, "	LXType_char,");
		}

		WriteLineToFile(propertyFile, "	LXType_None,");
		WriteLineToFile(propertyFile, "};");
		WriteLineToFile(propertyFile, "");

		// Property flags
		WriteLineToFile(propertyFile, "enum PropertyFlags");
		WriteLineToFile(propertyFile, "{");

		int power = 1;
		for (auto& it = g_PropertyFlagsNames.begin(); it != g_PropertyFlagsNames.end(); ++it)
		{
			PropertyFlags flag = it->first;
			std::string name = it->second;

			WriteLineToFile(propertyFile, "	" + name + " = " + std::to_string(power) + ",");

			power *= 2;
		}
		WriteLineToFile(propertyFile, "};");
		WriteLineToFile(propertyFile, "");

		// ClassPropertyData definition
		WriteLineToFile(propertyFile, R"(
// todo handle structs in .gen files
struct ClassPropertyData
{
	const char* m_Name;
	int m_Index;
	int m_Offset;
	int debug;
	LXType m_Type;
	bool IsAPointer;
	LXType m_AssociatedType;
	int m_PropertyFlags;
	float m_MinValue;
	float m_MaxValue;
};
		)");

		propertyFile.close();
	}

}
#endif

void DoMainProcessing(FileSelectionMode fileSelectionMode, std::string fileToForce)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;

	std::vector<std::string> srcFileList;

	hFind = FindFirstFile("../../../LigumX/src/*", &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			srcFileList.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));


		FindClose(hFind);
	}


	std::vector<GeneratorFile> generatorFiles;
	for (int fileIndex = 0; fileIndex < srcFileList.size(); ++fileIndex)
	{
		std::string& file = srcFileList[fileIndex];

		int dotIndex = file.find_first_of('.');

		std::string fileName = file.substr(0, dotIndex);

		if (fileSelectionMode == FileSelectionMode::SelectFile)
		{
			if (file != fileToForce)
			{
				continue;
			}
		}

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

			struct _stat64i32 result;
			int timeLastModified = 0;
			bool processFile = false;

			if (stat((g_GenerationRootDir + genFile.m_Name).c_str(), &result) == 0)
			{
				timeLastModified = (int)result.st_mtime;
				processFile = g_LogFile.ProcessFile(fileName, (int)timeLastModified);
			}

			if (processFile || (fileSelectionMode != FileSelectionMode::Normal))
			{
				std::cout << "\"" << fileName << "\" : has been updated." << std::endl;
				generatorFiles.push_back(genFile);
			}
			else
			{
				std::cout << "\"" << fileName << "\" : no change since " << timeLastModified << std::endl;
			}
		}
		else if (type == FileType_Header)
		{

		}
		else if (type == FileType_Source)
		{

		}
		else
		{
			__debugbreak();
		}

		if (fileSelectionMode == FileSelectionMode::OnlyFirst)
		{
			break;
		}

	}

	processGeneratorFiles(generatorFiles);

	g_LogFile.Save();

	PrintLine("Finished!");
	PrintLine("");
}

void DoMainProcessing(FileSelectionMode fileSelectionMode)
{
	DoMainProcessing(fileSelectionMode, "");
}

void OpenLogFile()
{
	std::stringstream cmd;
	cmd << "\"C:\\Program Files\\Sublime Text 3\\sublime_text.exe\" ";
	cmd << g_LogFile.m_FileName;
	system(cmd.str().c_str());
}

void MainLoop()
{
	char command = 0;

	while (command != 'q')
	{
		PrintLine("Input command.");
		PrintLine("Options : ");
		PrintLine("    (p)rocess");
		PrintLine("    (f)orce process all");
		PrintLine("    (o)nly first file");
		PrintLine("    (s)elect file (force)");
		PrintLine("    open (l)og file");
		PrintLine("    (q)uit");
		std::cin >> command;

		if (command == 'p')
		{
			DoMainProcessing(FileSelectionMode::Normal);
		}
		else if (command == 'f')
		{
			DoMainProcessing(FileSelectionMode::ForceAll);
		}
		else if (command == 'o')
		{
			DoMainProcessing(FileSelectionMode::OnlyFirst);
		}
		else if (command == 's')
		{
			PrintLine("Enter file name (without .gen) : ");
			char fileName[256];

			std::cin >> fileName;

			DoMainProcessing(FileSelectionMode::SelectFile, std::string(fileName) + ".gen");
		}
		else if (command == 'l')
		{
			OpenLogFile();
		}

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

	InitializeGenerator();

	MainLoop();

	//GeneratePropertyFile();

	return 0;
}

