#pragma once

#include <string>
#include <windows.h>
#include "stdafx.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cassert>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <stringutils.h>

enum CodeRegionType
{
	CodeRegionType_ClassForwardDecl,
	CodeRegionType_ClassHeader,
	CodeRegionType_ClassSource,
	CodeRegionType_NumTypes
};

enum FileType
{
	FileType_Source,
	FileType_Header,
	FileType_Generator,
	FileType_Invalid,
	FileType_NumFileTypes
};





const std::string g_CodeRegionBeginString = "#pragma region ";
const std::string g_CodeRegionEndString = "#pragma endregion ";
std::string g_GenerationRootDir;
bool bWriteCodeRegions = true;

std::unordered_map<std::type_index, int> g_TypeMap;

StringList UnwantedSubstrings; 
StringList AllowedTypesForGUI;

std::string g_PropertyArrayDeclaration = "static const ClassPropertyData g_"/* + m_Class.m_Name + */"Properties";