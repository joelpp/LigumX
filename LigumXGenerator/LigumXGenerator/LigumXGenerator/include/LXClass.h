#pragma once

#include "Common.h"
#include "LXEnum.h"

enum ClassPropertyFlags
{
	ClassPropertyFlags_PostSerialization = 1,
	ClassPropertyFlags_GlobalInstance	 = 2,
};

enum PropertyFlags
{
	PropertyFlags_Hidden			= 1 << 0,
	PropertyFlags_ReadOnly			= 1 << 1,
	PropertyFlags_SetCallback		= 1 << 2,
	PropertyFlags_GetCallback		= 1 << 3,
	PropertyFlags_MinValue			= 1 << 4,
	PropertyFlags_MaxValue			= 1 << 5,
	PropertyFlags_Transient			= 1 << 6,
	PropertyFlags_NonEditable		= 1 << 7,
	PropertyFlags_Adder				= 1 << 8,
	PropertyFlags_Enum				= 1 << 9,
};

std::unordered_map<std::string, PropertyFlags> g_PropertyFlagsStringMap;
std::unordered_map<PropertyFlags, std::string> g_PropertyFlagsNames;
std::unordered_map<std::string, ClassPropertyFlags> g_ClassPropertyFlagsStringMap;

struct Variable
{
	bool IsAPointer() const
	{
		return m_IsPtr;
	}

	bool IsGLMType() const
	{
		return stringContains(m_Type, "glm");
	}

	bool HasDefaultValue() const
	{
		return !(m_DefaultValue.empty());
	}

	void SetType(std::string fullTypeName)
	{
		m_Type = fullTypeName;
	}

	void RemoveTemplateDeclaration()
	{
		size_t start = m_Type.find('<');
		size_t end = m_Type.find('>');

		m_Type.erase(start, end+1);
	}

	void CheckForTemplate()
	{
		m_IsTemplate = stringContains(m_Type, '<');

		if (m_IsTemplate)
		{
			// for now we'll assume the associated type is always a pointer.
			m_AssociatedType = getSubstringBetweenCharacters(m_Type, '<', '>');

			// remove template declaration from string
			RemoveTemplateDeclaration();
		}

	}

	// todo : add .gen files to this project!
//private:
	std::string m_Name;
	std::string m_Type;
	std::string m_AssociatedType;
	bool m_IsPtr;
	bool m_IsTemplate;
	int m_PropertyFlags;

	std::string m_MinValue;
	std::string m_MaxValue;

	std::string m_DefaultValue;
};
typedef std::vector<Variable> VariableList;
typedef std::vector<LXEnum> EnumList;


struct LXClass
{
	std::string m_Name;
	std::string m_ParentName;
	std::string m_FileNames[FileType_NumFileTypes];
	int m_PropertyFlags;
	VariableList m_Members;
	EnumList m_Enums;
	bool m_IsValid = false;
};

typedef std::vector<LXClass> ClassList;
typedef std::vector<std::string> TokenList;


bool DiscardTokens(TokenList& tokens)
{
	return  (tokens[0] == "{") ||
		(tokens[0] == "") ||
		(tokens.size() == 0);
}

bool IsEnumDeclaration(TokenList& tokens)
{
	return tokens[0] == "enum";
}


bool IsClassDeclaration(TokenList& tokens)
{
	return tokens[0] == "class";
}

bool IsPropertyFlags(std::string& line)
{
	return stringContains(line, '[');
}

int GetVarPropertyFlags(TokenList& tokens, Variable& variable)
{
	int flags = 0;

	for (std::string& token : tokens)
	{
		std::string flagName = token;
		if (stringContains(token, '='))
		{
			TokenList list = splitString(token, '=');

			flagName = list[0];

			if (flagName == "min")
			{
				variable.m_MinValue = list[1];
			}

			if (flagName == "max")
			{
				variable.m_MaxValue = list[1];
			}
		}

		flags |= g_PropertyFlagsStringMap[flagName];
	}

	return flags;
}

int GetClassPropertyFlags(TokenList& tokens)
{
	int flags = 0;

	for (std::string& token : tokens)
	{
		std::string flagName = token;
		if (stringContains(token, '='))
		{
			TokenList list = splitString(token, '=');

			flagName = list[0];
		}

		flags |= g_ClassPropertyFlagsStringMap[flagName];
	}

	return flags;
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
