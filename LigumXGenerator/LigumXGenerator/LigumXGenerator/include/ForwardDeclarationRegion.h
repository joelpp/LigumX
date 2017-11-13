#pragma once


class ForwardDeclarationRegion : public CodeRegion
{
public:
	ForwardDeclarationRegion(LXClass& aClass)
		: CodeRegion(aClass)
	{
		m_IDString = "FORWARD_DECLARATIONS";
		m_FileExtension = ".h";
		m_CodeRegionBeginString = g_CodeRegionBeginString + " " + m_IDString + " " + m_Class.m_Name;
		m_CodeRegionEndString = g_CodeRegionEndString + " " + m_IDString + " " + m_Class.m_Name;
	}

	bool IsBeginMarker(TokenList& token) { return IsIncludeDeclaration(token); }

	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;

		std::vector<std::string> classesToDeclare;
		classesToDeclare.push_back(m_Class.m_Name);

		WriteLine("#include \"property.h\"");

		WriteLine("");

		// todo : add autoincludes from this
		for (Variable v : m_Class.m_Members)
		{
			bool isTemplate = v.m_IsTemplate;

			if (v.IsAPointer() || isTemplate)
			{
				std::string type;

				if (isTemplate)
				{
					type = v.m_AssociatedType;
				}
				else
				{
					type = v.m_Type;
				}


				if (!stringArrayContains(classesToDeclare, type))
				{
					classesToDeclare.push_back(type);
				}
			}
		}

		for (std::string& str : classesToDeclare)
		{
			WriteLine("class " + str + ";");
		}

		WriteLine("");

		if (m_Class.m_PropertyFlags & ClassPropertyFlags_GlobalInstance)
		{
			WriteLine("extern " + m_Class.m_Name + "* g_" + m_Class.m_Name + ";");
		}

	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}


	void WriteEnums()
	{
		for (LXEnum& anEnum : m_Class.m_Enums)
		{
			WriteLine("enum " + anEnum.m_Name);
			WriteLine("{");

			for (std::string& name : anEnum.m_Values)
			{
				WriteLine(anEnum.m_Name + "_" + name + ",");
			}
			WriteLine("NumItems_" + anEnum.m_Name);
			WriteLine("};");
			WriteLine("");

			WriteLine("extern const std::string EnumValues_" + anEnum.m_Name + "[" + std::to_string(anEnum.m_Values.size()) + "];");
			WriteLine("const int g_" + anEnum.m_Name + "EnumLength = " + std::to_string(anEnum.m_Values.size()) + ";");

			WriteLine("");
		}
	}
	void Process()
	{
		// Write class header region
		WriteHeader();
		WriteEnums();

		WriteFooter();

		WriteToFile();
	}
};
