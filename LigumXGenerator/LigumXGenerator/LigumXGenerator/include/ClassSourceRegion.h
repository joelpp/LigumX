#pragma once

#include "CodeRegion.h"

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

	void WriteDefaultConstructor()
	{
		if (m_Class.m_Name == "SunLight")
		{
			WriteLine(m_Class.m_Name + "::" + m_Class.m_Name + "() { }");

			WriteLine(m_Class.m_Name + "::" + m_Class.m_Name + "(");

			/* todo finish this eventually :effort:
			for (Variable& var : m_Class.m_Members)
			{
				Write()

			}*/

		}


	}

	std::string BuildPropertyFlagsString(int flags)
	{
		std::string toReturn = "";

		int numFlags = log2(flags);

		if (flags & PropertyFlags_Hidden)
		{
			toReturn += "PropertyFlags_Hidden";
		}

		if (toReturn.size() == 0)
		{
			toReturn = "0";
		}

		return toReturn;
	}

	void WritePropertyArray()
	{
		std::string propertyCountVarName = "g_" + m_Class.m_Name + "PropertyCount";
		int numProperties = m_Class.m_Members.size();

		WriteLine("const ClassPropertyData " + m_Class.m_Name + "::g_Properties[] = ");
		WriteLine("{");

		for (int i = 0; i < numProperties; ++i)
		{
			Variable& var = m_Class.m_Members[i];
			std::string&  varName = var.m_Name;
			std::string&  varType = var.m_Type;
			// warning! if you change anything here mirror it in property.h in LigumX
			WriteLine("{ \"" + varName + "\", "
				+ "offsetof(" + m_Class.m_Name + ", m_" + varName + "), "
				+ (m_Class.m_Members[i].m_Type == "\tbool" ? "1" : "0") + ", "
				+ "LXType_" + RemoveSubstrings(varType, "::") + ", "
				+ (var.IsAPointer() ? "true" : "false") + ", "
				+ (var.m_IsTemplate ? ("LXType_" + var.m_AssociatedType) : "LXType_None") + ", "
				+ BuildPropertyFlagsString(var.m_PropertyFlags) + ", "
				+ " }, ");
		}


		WriteLine("};");
	}

	void WriteBody()
	{
		WriteLine("#include \"" + m_Class.m_Name + ".h\"");
		WriteLine("#include <cstddef>");
		
		WritePropertyArray();

		//WriteDefaultConstructor();
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
