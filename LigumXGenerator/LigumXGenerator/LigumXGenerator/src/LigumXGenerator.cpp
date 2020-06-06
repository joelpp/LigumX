// LigumXGenerator.cpp : Defines the entry point for the console application.
//
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

#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

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

//std::unordered_map<std::string, std::vector<std::string>> g_ParentToChildClasses;
std::vector<LXClass> g_AllClasses;
std::vector<std::string> g_EnumNames;

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
	bool m_SourceFilesNeedUpdate;
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
void PrintLine(const std::string& str)
{
	PrintLine(str.c_str());
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
		ELSE_RETURN_DEFAULT("glm::highp_ivec2", "glm::highp_ivec2(0, 0)")
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

					//if (!currentClass.m_ParentName.empty())
					{
					}

					if (currentClass.m_ParentName.empty())
					{
						if (currentClass.m_Name != "LXObject")
						{
							currentClass.m_ParentName = "LXObject";
						}
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

				variable.m_DefaultValue = "";
				for (int t = defaultValueTokenIndex; t < tokens.size(); ++t)
				{
					variable.m_DefaultValue += tokens[t];
					
					// if not last, add comma between values
					if (t != (tokens.size() - 1))
					{
						variable.m_DefaultValue += ", ";
					}
				}

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
	else
	{
		g_AllClasses.push_back(theClass);
	}
	
	for (const LXEnum& lxEnum : theClass.m_Enums)
	{
		g_EnumNames.push_back(lxEnum.m_Name);
	}

	if (genFile.m_SourceFilesNeedUpdate)
	{
		ClassHeaderRegion headerRegion(theClass);
		headerRegion.Process();

		ForwardDeclarationRegion forwardRegion(theClass);
		forwardRegion.Process();

		ClassSourceRegion sourceRegion(theClass);
		sourceRegion.Process();
	}

}

void processGeneratorFiles(std::vector<GeneratorFile>& generatorFiles)
{
	for (GeneratorFile& genFile : generatorFiles)
	{
		processSingleGeneratorFile(genFile);
	}
}

void OutputClassHierarchyData(std::vector<GeneratorFile>& generatorFiles)
{
#if 1
	std::stringstream classListFile;
	std::stringstream allClassIncludeFile;
	for (LXClass& theClass : g_AllClasses)
	{
		std::string parentName = (theClass.m_Name == "LXObject") ? "None" : theClass.m_ParentName;
		classListFile << "LX_CLASS(" << theClass.m_Name << ", " << parentName << ")" << std::endl;
		allClassIncludeFile << "#include \"" << theClass.m_Name << ".h\"" << std::endl;
	}

	for (std::string& enumName : g_EnumNames)
	{
		classListFile << "LX_ENUM(" << enumName << ")" << std::endl;
	}

	// output class list .h
	{
		std::string filePath = g_GenerationRootDir + "LXClassList.h";
		std::string lastClassList = StringFromFile(filePath.c_str());
		
		std::string newClassList = classListFile.str();
		bool contentsChanged = lastClassList != newClassList;
		if (contentsChanged)
		{
			std::fstream file(filePath.c_str(), std::fstream::out | std::fstream::trunc);

			if (file.is_open())
			{
				file << newClassList;
				file.close();
			}
		}

	}

	// output LXAllClassInclude .h
	{
		std::string filePath = g_GenerationRootDir + "LXAllClassInclude.h";
		std::string lastAllClassInclude = StringFromFile(filePath.c_str());

		std::string newAllClassInclude = allClassIncludeFile.str();
		bool contentsChanged = lastAllClassInclude != newAllClassInclude;
		if (contentsChanged)
		{
			std::fstream file(filePath.c_str(), std::fstream::out | std::fstream::trunc);

			if (file.is_open())
			{
				file << allClassIncludeFile.str();
				file.close();
			}
		}
	}


#else
	std::stringstream objectFactoryFile;
	objectFactoryFile << "#include \"ObjectFactory.h\"" << std::endl;
	for (std::string& childName : g_ChildClassesNames)
	{
		objectFactoryFile << "#include \"" << childName << ".h\"" << std::endl;
	}
	
	objectFactoryFile << std::endl;

	objectFactoryFile << "LXObject* ObjectFactory::GetNewObject(int hash, int objectID)" << std::endl;
	objectFactoryFile << "{" << std::endl;
	objectFactoryFile << "    switch (hash)" << std::endl;
	objectFactoryFile << "    {" << std::endl;

	for (std::string& childName : g_ChildClassesNames)
	{
		objectFactoryFile << "    case " << childName << "::ClassID:" << std::endl;
		objectFactoryFile << "    {" << std::endl;
		objectFactoryFile << "    " << childName << "* obj = new " << childName << "();" << std::endl;
		objectFactoryFile << "    obj->SetObjectID(objectID);" << std::endl;
		objectFactoryFile << "    obj->Serialize(false);" << std::endl;
		objectFactoryFile << "    g_ObjectManager->AddObject(objectID, obj->Type, (ObjectPtr)obj);" << std::endl;
		objectFactoryFile << "    return (LXObject*)obj;" << std::endl;
		objectFactoryFile << "	}" << std::endl;
	}
	objectFactoryFile << "    default:break;" << std::endl;

	objectFactoryFile << "    }" << std::endl;
	objectFactoryFile << "	return nullptr;" << std::endl;
	objectFactoryFile << "}" << std::endl;

	std::string filePath = g_GenerationRootDir + "ObjectFactory.cpp";
	std::fstream file(filePath.c_str(), std::fstream::out);

	if (file.is_open())
	{
		file << objectFactoryFile.str();
		file.close();
	}
#endif
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
	EMPLACE_PROPERTY_FLAG(PropertyFlags_AddCallback, "addcallback");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_MinValue, "min");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_MaxValue, "max");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Transient, "transient");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_NoCloning, "nocloning");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_NonEditable, "noneditable");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Adder, "adder");
	EMPLACE_PROPERTY_FLAG(PropertyFlags_Enum, "enum");

	g_ClassPropertyFlagsStringMap.emplace("postserialization", ClassPropertyFlags_PostSerialization);
	g_ClassPropertyFlagsStringMap.emplace("globalinstance", ClassPropertyFlags_GlobalInstance);

	g_LXTypeToImguiCallName.emplace("bool", "Bool");
	g_LXTypeToImguiCallName.emplace("int", "Int");
	g_LXTypeToImguiCallName.emplace("float", "Float");
	g_LXTypeToImguiCallName.emplace("lxInt64", "Int64");
	g_LXTypeToImguiCallName.emplace("glm::ivec2", "IVec2");
	g_LXTypeToImguiCallName.emplace("glm::ivec3", "IVec3");
	g_LXTypeToImguiCallName.emplace("glm::ivec4", "IVec4");
	g_LXTypeToImguiCallName.emplace("glm::highp_ivec2", "Highp_IVec2");
	g_LXTypeToImguiCallName.emplace("glm::vec2", "Vec2");
	g_LXTypeToImguiCallName.emplace("glm::vec3", "Vec3");
	g_LXTypeToImguiCallName.emplace("glm::vec4", "Vec4");
	g_LXTypeToImguiCallName.emplace("std::string", "String");
	g_LXTypeToImguiCallName.emplace("std::vector", "Vector");

	g_DefaultMinForType.emplace("int", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("lxInt64", "LX_LIMITS_INT64_MIN");
	g_DefaultMinForType.emplace("glm::ivec2", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("glm::ivec3", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("glm::ivec4", "LX_LIMITS_INT_MIN");
	g_DefaultMinForType.emplace("glm::highp_ivec2", "LX_LIMITS_INT64_MIN");
	g_DefaultMinForType.emplace("float", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec2", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec3", "LX_LIMITS_FLOAT_MIN");
	g_DefaultMinForType.emplace("glm::vec4", "LX_LIMITS_FLOAT_MIN");

	g_DefaultMaxForType.emplace("int", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("lxInt64", "LX_LIMITS_INT64_MAX");
	g_DefaultMaxForType.emplace("glm::ivec2", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("glm::ivec3", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("glm::ivec4", "LX_LIMITS_INT_MAX");
	g_DefaultMaxForType.emplace("glm::highp_ivec2", "LX_LIMITS_INT64_MAX");
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

			if ((fileSelectionMode != FileSelectionMode::ForceAll) && stat((g_GenerationRootDir + genFile.m_Name).c_str(), &result) == 0)
			{
				timeLastModified = (int)result.st_mtime;
				genFile.m_SourceFilesNeedUpdate = g_LogFile.ProcessFile(fileName, (int)timeLastModified);
				if (!genFile.m_SourceFilesNeedUpdate)
				{
					std::cout << "\"" << fileName << "\" : no change since " << timeLastModified << std::endl;
				}
			}

			std::cout << "\"" << fileName << "\" : has been updated." << std::endl;
			generatorFiles.push_back(genFile);
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

	OutputClassHierarchyData(generatorFiles);

	g_LogFile.Save();
	g_EnumNames.clear();
	g_AllClasses.clear();


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

const std::string g_PathMainData = "C:/Users/User/Documents/Code/LigumX/LigumX/data/";
const std::string g_PathObjects = g_PathMainData + "objects/";

struct FileDisplayInformation
{
	long int m_ObjectID;
	std::string m_Typename;
	std::string m_Name;
	std::string m_AsText;

	FileDisplayInformation(const std::string& fileName);
};
FileDisplayInformation::FileDisplayInformation(const std::string& fileName)
{
	bool valid = false;
	std::vector<std::string> all = splitString(fileName, '_');

	if (all.size() == 2)
	{
		std::vector<std::string> idType = splitString(all[1], '.');

		if (idType.size() == 2)
		{
			m_ObjectID = atoi(idType[0].c_str());
			m_Typename = all[0];
			valid = true;
		}
	}

	//lxAssert(valid);
	if (valid)
	{
		// go get the name
		// todo jpp : refactor this into a kind of "get data value from file"
		std::fstream file = std::fstream((g_PathObjects + fileName).c_str(), std::fstream::in);

		if (file.is_open())
		{
			std::string line;
			bool foundName = false;
			while (std::getline(file, line))
			{
				if (line == "Name" && !foundName)
				{
					foundName = true;
					continue;
				}
				if (foundName)
				{
					m_Name = line;
					break;
				}
			}
			file.close();
		}
		m_AsText = m_Typename + " (" + std::to_string(m_ObjectID) + ") : " + m_Name;
	}
}

std::vector<std::string> GetAllFilesInDirectory(const char* path)
{
	std::vector<std::string> out;

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind;

	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	//StringCchLength(path, MAX_PATH, &length_of_arg);
	StringCchCopy(szDir, MAX_PATH, path);
	StringCchCat(szDir, MAX_PATH, TEXT("*"));

	hFind = FindFirstFileA(szDir, &ffd);

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			std::string outPath = std::string(ffd.cFileName);
			out.push_back(outPath);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);

	return out;

}

std::vector<FileDisplayInformation> GetAllFiles()
{

	std::vector<FileDisplayInformation> files;
	StringList allFileNames = GetAllFilesInDirectory(g_PathObjects.c_str());

	for (std::string& fileName : allFileNames)
	{
		files.push_back(FileDisplayInformation(fileName));
	}

	return files;
}


void RunDataMigrationTool()
{
	enum SourceDataOperation
	{
		SourceDataOperation_Replace,
		SourceDataOperation_Add,
	};

	char userInput[256];

	PrintLine("Enter class type to migrate FROM (source):");
	std::cin >> userInput;
	std::string classSource = std::string(userInput);

	PrintLine("Enter member name to remove:");
	std::cin >> userInput;
	std::string classMember = std::string(userInput);

	PrintLine("Enter member name to add:");
	std::cin >> userInput;
	std::string classMemberNew = std::string(userInput);

	PrintLine("Enter class type to migrate TO (destination):");
	std::cin >> userInput;
	std::string classDest = std::string(userInput);

	PrintLine("Enter source data operation (0 = replace, 1 = add):");
	std::cin >> userInput;
	SourceDataOperation operation = (SourceDataOperation)std::atoi(userInput);

	std::vector<FileDisplayInformation> allFiles = GetAllFiles();

	long int nextID = 0;
	for (FileDisplayInformation& str : allFiles)
	{
		long int id = str.m_ObjectID;

		nextID = std::max(id, nextID);
	}

	nextID++;

	for (int i = 0; i < allFiles.size(); ++i)
	{
		const FileDisplayInformation& info = allFiles[i];




		if (info.m_Typename == classSource)
		{
			PrintLine(std::string("Processing file ") + info.m_Name);

			std::string inName = g_PathObjects + info.m_Typename + "_" + std::to_string(info.m_ObjectID) + ".LXObj";
			std::vector<std::string> lines = readFileLines(inName.c_str());

			std::string oldValue = "";

			bool foundToRemove = false;
			bool hasNewMember = false;
			std::stringstream newLines;
			for (int i = 0; i < lines.size(); ++i)
			{
				if (!foundToRemove)
				{
					if (lines[i] == classMember)
					{
						foundToRemove = true;
					}
					if (lines[i] == classMemberNew)
					{
						hasNewMember = true;

						newLines << lines[i] << std::endl;

						bool nextLineIsNewMember = false;
						bool isArraySize = true;
						while (!nextLineIsNewMember)
						{
							if ((i + 1) >= lines.size())
							{
								break;
							}

							nextLineIsNewMember = std::isalpha(lines[i + 1][0]);

							if (isArraySize)
							{
								int val = std::atoi(lines[i + 1].c_str());
								val++;
								newLines << val << std::endl;
								isArraySize = false;
							}
							else
							{
								newLines << lines[i + 1] << std::endl;
							}
							i++;
						}

						newLines << nextID << std::endl;

					}
					else
					{
						newLines << lines[i] << std::endl;
					}
				}
				else
				{
					oldValue = lines[i];
					foundToRemove = false;
					continue;
				}
			}
			
			if (!hasNewMember)
			{
				newLines << classMemberNew << std::endl;
				newLines << nextID << std::endl;
			}

			if (oldValue != "")
			{
				std::ofstream out(inName.c_str());

				if (out.is_open())
				{
					out << newLines.str().c_str();
					out.close();
				}

				std::stringstream newFile;
				newFile << "Name" << std::endl;
				newFile << classDest << "_" << nextID << "_MigratedFrom_" << info.m_Typename << "_" << info.m_ObjectID << std::endl;
				newFile << "ObjectID" << std::endl;
				newFile << nextID << std::endl;
				newFile << classMember << std::endl;
				newFile << oldValue << std::endl;

				std::string outPath = g_PathObjects + classDest + "_" + std::to_string(nextID) + ".LXObj";

				std::ofstream out2(outPath.c_str());

				if (out2.is_open())
				{
					out2 << newFile.str().c_str();
					out2.close();
				}

				nextID++;
			}



		}
	}

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
		PrintLine("    (c)reate file");
		PrintLine("    open (l)og file");
		PrintLine("    (m)igrate existing data");
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
		else if (command == 'c')
		{
			PrintLine("Enter file name (without .gen) : ");
			char fileNameC[256];

			std::cin >> fileNameC;

			std::string fileName = std::string(fileNameC);

			if (fileName.size() > 0)
			{
				std::stringstream fileContents;
				fileContents << "\n";
				fileContents << "class " << fileName << "\n";
				fileContents << "{ \n";
				fileContents << "\n";
				fileContents << "} \n";
				fileContents << "\n";

				std::string filePath = g_GenerationRootDir + fileName + ".gen";

				std::fstream newFile(filePath.c_str(), std::fstream::out);
				if (newFile.is_open())
				{
					newFile << fileContents.str();
					newFile.close();
				}
			}
			else
			{
				PrintLine("ERROR : Filename can not be empty.");
			}
		}
		else if (command == 'l')
		{
			OpenLogFile();
		}
		else if (command == 'm')
		{
			RunDataMigrationTool();
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

