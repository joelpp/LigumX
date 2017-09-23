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

	void WriteBody()
	{
		WriteLine("#include \"" + m_Class.m_Name + ".h\"");
		WriteLine("#include <cstddef>");


		std::string propertyCountVarName = "g_" + m_Class.m_Name + "PropertyCount";
		int numProperties = m_Class.m_Members.size();

		WriteLine("const ClassPropertyData " + m_Class.m_Name + "::g_" + m_Class.m_Name + "Properties[] = ");
		WriteLine("{");

		for (int i = 0; i < numProperties; ++i)
		{
			Variable& var = m_Class.m_Members[i];
			std::string&  varName = var.m_Name;
			std::string&  varType = var.m_Type;

			// warning! if you change anything here mirror it in property.h in LigumX
			WriteLine( "{ \"" + varName + "\", " 
							  + "offsetof(" + m_Class.m_Name + ", m_" + varName + "), " 
							  + (m_Class.m_Members[i].m_Type == "\tbool" ? "1" : "0") + ", "
							  + "LXType_" + RemoveSubstrings(varType, "::") + ", " 
							  + ( var.IsAPointer() ? "true" : "false") + ", "  
							  + " }, " );
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
