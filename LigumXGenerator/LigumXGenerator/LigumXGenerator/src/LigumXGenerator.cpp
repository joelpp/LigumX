// LigumXGenerator.cpp : Defines the entry point for the console application.
//
#define NOMINMAX
#include "stdafx.h"
#include "Common.h"
#include "LXClass.h"
#include "CodeRegion.h"
#include "ClassSourceRegion.h"
#include "ClassHeaderRegion.h"
#include "ForwardDeclarationRegion.h"

#define NAMESPACE_BEGIN namespace LigumXGenerator {
#define NAMESPACE_END }


struct GeneratorFile
{
	std::string m_Name;
	std::vector<LXClass> m_Contained;
};



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

			if (IsMethodDeclaration(tokens))
			{
				// todo implement
				continue;
			}

			if (IsClassDeclaration(tokens))
			{
				currentClass.m_Name = tokens[1];
				generatingClass = true;

				Variable objectIDVar;
				objectIDVar.m_IsPtr = false;
				objectIDVar.m_IsTemplate = false;
				objectIDVar.m_Name = "ObjectID";
				objectIDVar.m_Type = "int";
				objectIDVar.m_PropertyFlags = 0;
				currentClass.m_Members.push_back(objectIDVar);

				Variable nameVar;
				nameVar.m_IsPtr = false;
				nameVar.m_IsTemplate = false;
				nameVar.m_Name = "Name";
				nameVar.m_Type = "std::string";
				nameVar.m_PropertyFlags = 0;
				currentClass.m_Members.push_back(nameVar);

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

			variable.m_Name = tokens[1];;
			variable.SetType(tokens[0]);

			variable.m_IsPtr = stringContains(variable.m_Type, '*');

			variable.CheckForTemplate();

			variable.m_Type.erase(std::remove(variable.m_Type.begin(), variable.m_Type.end(), '*'), variable.m_Type.end());

			variable.m_PropertyFlags = varPropertyFlags;

			currentClass.m_Members.push_back(variable);
			variable = Variable();
			// not invalid or a class declaration. we're adding to the class desc
			void;

			// reset property flags
			varPropertyFlags = 0;
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

void SystemPause()
{
	system("pause");
}

void InitializeGenerator()
{
	// Substrings we want to remove from property names
	g_UnwantedSubstrings.push_back("\t");
	g_UnwantedSubstrings.push_back(";");
	g_UnwantedSubstrings.push_back("[");
	g_UnwantedSubstrings.push_back("]");
	g_UnwantedSubstrings.push_back(",");

	// Type names that have a corresponding gui callback
	g_AllowedTypesForGUI.push_back("bool");
	g_AllowedTypesForGUI.push_back("glm::vec3");

	g_PropertyFlagsStringMap.emplace("hidden", PropertyFlags_Hidden);
	g_PropertyFlagsStringMap.emplace("readonly", PropertyFlags_ReadOnly);
	g_PropertyFlagsStringMap.emplace("setcallback", PropertyFlags_SetCallback);
	g_PropertyFlagsStringMap.emplace("min", PropertyFlags_MinValue);
	g_PropertyFlagsStringMap.emplace("max", PropertyFlags_MaxValue);
	g_PropertyFlagsStringMap.emplace("transient", PropertyFlags_Transient);
	g_PropertyFlagsStringMap.emplace("noneditable", PropertyFlags_NonEditable);

	g_ClassPropertyFlagsStringMap.emplace("postserialization", ClassPropertyFlags_PostSerialization);
}

int main()
{
	InitializeGenerator();

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

