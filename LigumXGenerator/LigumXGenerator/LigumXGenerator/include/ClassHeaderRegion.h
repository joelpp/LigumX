#pragma once

#include "CodeRegion.h"

class ClassHeaderRegion : public CodeRegion
{
public:
	ClassHeaderRegion(LXClass& aClass)
		: CodeRegion(aClass)
	{
		m_IDString = "HEADER";
		m_FileExtension = ".h";
		m_CodeRegionBeginString = g_CodeRegionBeginString + " " + m_IDString + " " + m_Class.m_Name;
		m_CodeRegionEndString = g_CodeRegionEndString + " " + m_IDString + " " + m_Class.m_Name;
	}
	bool IsBeginMarker(TokenList& token)
	{
		bool ok = false;
		bool header = IsClassDeclaration(token);

		if (header && token.size() > 1 && token[1] == m_Class.m_Name)
		{
			ok = true;
		}
		return ok;
	}


	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;
		m_Stream << "static const int ClassID = " << std::hash_value(m_Class.m_Name) << ";";
		m_Stream << std::endl;
	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}

	void WriteClassVariableGettersAndSetters()
	{
		m_Stream << "public:";
		m_Stream << std::endl;
		for (Variable var : m_Class.m_Members)
		{
			// Write getter
			bool ptr = var.IsAPointer();
			m_Stream << (ptr ? "" : "const ");
			m_Stream << (var.m_Type);

			if (var.m_IsTemplate)
			{
				m_Stream << "<";
				m_Stream << var.m_AssociatedType << "*";
				m_Stream << ">";
			}

			m_Stream << (ptr ? "*" : "&");
			m_Stream << (" ");
			m_Stream << "Get" << var.m_Name << "() { return m_" << var.m_Name << "; }; ";
			m_Stream << std::endl;

			// Write setter
			m_Stream << "void Set" << var.m_Name << "(" << var.m_Type;

			if (var.m_IsTemplate)
			{
				m_Stream << "<";
				m_Stream << var.m_AssociatedType << "*";
				m_Stream << ">";
			}

			m_Stream << (ptr ? "*" : "");
			m_Stream << " value) { m_" << var.m_Name << " = value; };";
			m_Stream << std::endl;
		}
	}

	void WriteProperties()
	{
		WriteLine("public:");

		// TODO : functions to compute this or store it
		std::string propertyCountVarName = "g_" + m_Class.m_Name + "PropertyCount";
		int numProperties = m_Class.m_Members.size();
		WriteLine("static const int " + propertyCountVarName + " = " + std::to_string(numProperties) + ";");

		WriteLine(g_PropertyArrayDeclaration + "[" + propertyCountVarName + "];");

		m_Stream << std::endl;
	}

	void WriteDefaultConstructor()
	{
		if (m_Class.m_Name == "SunLight") // todo remove hack
			WriteLine(m_Class.m_Name + "::" + m_Class.m_Name + "();");

	}


	void WriteBody()
	{
		WriteClassVariableGettersAndSetters();

		m_Stream << "private:" << std::endl;
		for (Variable var : m_Class.m_Members)
		{
			m_Stream << var.m_Type;

			if (var.m_IsTemplate)
			{
				m_Stream << "<";
				m_Stream << var.m_AssociatedType << "*";
				m_Stream << ">";
			}

			m_Stream << (var.IsAPointer() ? "*" : "");
			m_Stream << " m_" << var.m_Name << ";";
			m_Stream << std::endl;
		}

		WriteProperties();
		WriteDefaultConstructor();
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