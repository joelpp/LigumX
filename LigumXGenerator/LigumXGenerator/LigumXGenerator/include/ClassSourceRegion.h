#pragma once

#include "CodeRegion.h"


// count the no. of set bits in a positive integer
int countSetBits(unsigned int num) {
	unsigned int count = 0;
	while (num) {
		// bitwise AND operation to check if the
		// leftmost bit is set or not
		// if set, increment the count
		count += num & 1;
		// left shift the nm by one position
		num >>= 1;
	}
	return count;
}

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

	std::string BuildPropertyFlagsString(unsigned int flags)
	{
		std::string toReturn = "";

		int numFlags = countSetBits(flags);

		std::string separator = " | ";

		//for (int i = 1; i < (PropertyFlags_NumPropertyFlags / 2); i *= 2)
		//{

			//if (flags & (PropertyFlags)i)
		if (flags & PropertyFlags_Hidden)
		{
			toReturn += "PropertyFlags_Hidden";
			toReturn += separator;
		}

		if (flags & PropertyFlags_SetCallback)
		{
			toReturn += "PropertyFlags_SetCallback";
			toReturn += separator;
		}

			//if (i != numFlags)
			//{
			//	toReturn += " | ";
			//}
		//}


		if (toReturn.size() == 0)
		{
			toReturn = "0";
		}
		else
		{
			toReturn.erase(toReturn.size() - separator.size(), toReturn.size());
		}

		return toReturn;
	}

	void WritePropertyArray()
	{
		std::string propertyCountVarName = "g_PropertyCount";
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
				+ (var.m_MinValue.size() > 0 ? var.m_MinValue : "0") + ", "
				+ (var.m_MaxValue.size() > 0 ? var.m_MaxValue : "0") + ", "
				+ "}, ");
		}


		WriteLine("};");
	}

	void WriteSerializer()
	{
		WriteLine("void " + m_Class.m_Name + "::Serialize(bool writing)");
		WriteLine("{");
		WriteLine(R"(	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";)");
		WriteLine("	std::string fileName = \"" + m_Class.m_Name + ".LXobj\";");

		WriteLine(R"(
	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		if (objectStream.is_open())
		{
			Serializer::SerializeObject(this, objectStream, writing);
		}
	}
})");
	}

	void WriteBody()
	{
		WriteLine("#include \"" + m_Class.m_Name + ".h\"");
		WriteLine("#include \"serializer.h\"");
		WriteLine("#include <cstddef>");
		
		WritePropertyArray();

		// todo unleash for all LXclasses
		if (m_Class.m_Name == "DisplayOptions" || 
			m_Class.m_Name == "PostEffects")
		{
			WriteSerializer();
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

		WriteBody();

		WriteFooter();

		WriteToFile();
	}
};
