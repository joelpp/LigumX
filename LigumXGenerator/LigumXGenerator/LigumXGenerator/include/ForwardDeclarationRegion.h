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
		WriteLine("#include \"property.h\"");

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

		WriteFooter();

		WriteToFile();
	}
};
