#pragma once
#include <algorithm>

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

	bool IsBeginMarker(TokenList& token, bool createStub) 
	{ 
		return IsIncludeDeclaration(token); 
	}

	void WriteHeader()
	{
		m_Stream << m_CodeRegionBeginString;
		m_Stream << std::endl;

		std::vector<std::string> classesToDeclare;
		classesToDeclare.push_back(m_Class.m_Name);

		WriteLine("#include \"LXSystem.h\"");
		WriteLine("class Serializer2;");

		WriteLine("");

		std::vector<std::string> excludedClasses =
		{
			"std::string",
			"bool",
			"int",
			"float",
			"glm::vec2",
			"glm::vec3",
			"glm::vec4",
			"glm::mat4",
		};

		// todo : add autoincludes from this
		for (const Variable& v : m_Class.m_Members)
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
					type = v.GetType();
				}

				bool excludedClass = (std::find(excludedClasses.begin(), excludedClasses.end(), type) != excludedClasses.end());
				if (!stringArrayContains(classesToDeclare, type) && !excludedClass)
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

			int i = 0;
			for (std::string& name : anEnum.m_Values)
			{
				Write(anEnum.m_Name + "_" + name);

				if (anEnum.m_DefaultValues[i] != "")
				{
					Write(" = " + anEnum.m_DefaultValues[i]);
				}

				WriteLine(",");
				i++;
			}
			WriteLine("};");
			WriteLine("");

			std::string sizeBrackets = "[" + std::to_string(anEnum.m_Values.size()) + "]";
			WriteLine("extern const std::string EnumValues_" + anEnum.m_Name + sizeBrackets + ";");
			WriteLine("extern const " + anEnum.m_Name + " Indirection_" + anEnum.m_Name + sizeBrackets + ";");
			WriteLine("const int EnumLength_" + anEnum.m_Name + " = " + std::to_string(anEnum.m_Values.size()) + ";");

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
