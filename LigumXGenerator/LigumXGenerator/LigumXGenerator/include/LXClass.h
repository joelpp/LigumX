#pragma once

#include "Common.h"


struct Variable
{
	bool IsAPointer()
	{
		return m_IsPtr;
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

	// todo : add .gen files to this project!
//private:
	std::string m_Name;
	std::string m_Type;
	std::string m_AssociatedType;
	bool m_IsPtr;
};
typedef std::vector<Variable> VariableList;


struct LXClass
{
	std::string m_Name;
	std::string m_FileNames[FileType_NumFileTypes];
	VariableList m_Members;
};
typedef std::vector<LXClass> ClassList;
typedef std::vector<std::string> TokenList;


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
