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
	PropertyFlags_AddCallback		= 1 << 4,
	PropertyFlags_MinValue			= 1 << 5,
	PropertyFlags_MaxValue			= 1 << 6,
	PropertyFlags_Transient			= 1 << 7,
	PropertyFlags_NonEditable		= 1 << 8,
	PropertyFlags_Adder				= 1 << 9,
	PropertyFlags_Enum				= 1 << 10,
	PropertyFlags_NoCloning			= 1 << 11,
};


std::unordered_map<std::string, PropertyFlags> g_PropertyFlagsStringMap;
std::unordered_map<PropertyFlags, std::string> g_PropertyFlagsNames;
std::unordered_map<std::string, ClassPropertyFlags> g_ClassPropertyFlagsStringMap;
std::unordered_map<std::string, std::string> g_LXTypeToImguiCallName;
std::unordered_map<std::string, std::string> g_DefaultMinForType;
std::unordered_map<std::string, std::string> g_DefaultMaxForType;


struct Variable
{
	Variable()
	{

	}
	void SetMinValue(const std::string& value)
	{
		m_MinValue = value;
	}

	void SetMaxValue(const std::string& value)
	{
		m_MaxValue = value;
	}

	std::string GetMinValue()
	{
		if (!m_MinValue.empty())
		{
			return m_MinValue;
		}

		auto defaultMinIt = g_DefaultMinForType.find(m_Type);

		if (defaultMinIt == g_DefaultMinForType.end())
		{
			return "0";
		}
		else
		{
			return defaultMinIt->second;
		}
	}

	std::string GetMaxValue()
	{
		if (!m_MaxValue.empty()) 
		{
			return m_MaxValue;
		}

		auto defaultMaxIt = g_DefaultMaxForType.find(m_Type);

		if (defaultMaxIt == g_DefaultMaxForType.end())
		{
			return "0";
		}
		else
		{
			return defaultMaxIt->second;
		}
	}

	bool IsAPointer() const
	{
		return m_IsPtr;
	}

	bool IsGLMType() const
	{
		return stringContains(m_Type, "glm");
	}
	bool IsPrimitive() const
	{
		return m_IsPrimitive;
	}

	bool IsString() const
	{
		return m_Type == "std::string";
	}

	bool HasDefaultValue() const
	{
		return !(m_DefaultValue.empty());
	}

	void SetType(const std::string& fullTypeName)
	{
		m_Type = fullTypeName;

		m_IsPrimitive = (m_Type == "float") || (m_Type == "int") || (m_Type == "bool");

		m_MinValue = GetMinValue();
		m_MaxValue = GetMaxValue();
	}

	void RemoveTemplateDeclaration()
	{
		size_t start = m_Type.find('<');
		size_t end = m_Type.find('>');

		if (start != std::string::npos)
		{
			m_Type.erase(start, end + 1);
		}
	}

	void CheckForTemplate()
	{
		m_IsTemplate = stringContains(m_Type, '<');

		if (m_IsTemplate)
		{
			// for now we'll assume the associated type is always a pointer.
			m_AssociatedType = getSubstringBetweenCharacters(m_Type, '<', '>');

			if (m_AssociatedType.find('*') != std::string::npos)
			{
				m_AssociatedPtr = true;
				m_AssociatedType = RemoveSubstrings(m_AssociatedType, "*");
			}

			// remove template declaration from string
			RemoveTemplateDeclaration();
		}

	}

	const std::string& GetType() const { return m_Type; }

	// todo : add .gen files to this project!
//private:
	std::string m_Name;
	std::string m_AssociatedType;
	bool m_AssociatedPtr = false;
	bool m_IsPtr;
	bool m_IsTemplate;
	bool m_IsPrimitive = false;
	int m_PropertyFlags;

	std::string m_DefaultValue;

private:
	std::string m_Type;
	std::string m_MinValue;
	std::string m_MaxValue;

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

	bool IsLXObject() 
	{
		return m_ParentName.empty();
	}
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
			std::string s = list[1];

			// ugly hack because we always store min/max values as float
			// probably better would be store a list of "attribute" objects and have a class FloatMaxAttribute : public GenAttribute
			if (!stringContains(s, ".f"))
			{
				s += ".f";
			}

			if (flagName == "min")
			{
				variable.SetMinValue(s);
			}
			else if (flagName == "max")
			{
				variable.SetMaxValue(s);
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
