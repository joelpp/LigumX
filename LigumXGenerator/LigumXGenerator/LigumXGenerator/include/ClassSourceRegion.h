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

#define ADD_PROPERTY(propertyflag) \
if (flags & propertyflag) \
{ \
	toReturn += #propertyflag; \
	toReturn += separator; \
} \


bool VarTypeSupportsLimits(const Variable& var)
{
	const std::string& varType = var.GetType();
	return (varType == "int")
		|| (varType == "long")
		|| (varType == "float")
		|| (varType == "double")
		|| (var.IsGLMType());

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

	bool IsBeginMarker(TokenList& token, bool createStub) { return IsIncludeDeclaration(token); }

	bool GenerateFileStub(std::string& stub)
	{
		std::stringstream ss;

		ss << "#include \"" << m_Class.m_Name << ".h\"";
		ss << "\n";
		ss << "\n";

		stub = ss.str();
		return true;
	}

	void WriteHeader()
	{
		WriteLine(m_CodeRegionBeginString);

		if (m_Class.m_PropertyFlags & ClassPropertyFlags_GlobalInstance)
		{
			WriteLine(m_Class.m_Name + "* g_" + m_Class.m_Name + ";");
		}

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

		// todo mapping between enum and strings
		// generated with lxgen
		ADD_PROPERTY(PropertyFlags_Hidden)
		ADD_PROPERTY(PropertyFlags_SetCallback)
		ADD_PROPERTY(PropertyFlags_Transient)
		ADD_PROPERTY(PropertyFlags_NonEditable)
		ADD_PROPERTY(PropertyFlags_Adder)
		ADD_PROPERTY(PropertyFlags_Enum)

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

		if (numProperties > 0)
		{
			WriteLine("const ClassPropertyData " + m_Class.m_Name + "::g_Properties[] = ");
			WriteLine("{");

			for (int i = 0; i < numProperties; ++i)
			{
				Variable& var = m_Class.m_Members[i];
				std::string&  varName = var.m_Name;
				const std::string&  varType = var.GetType();

				std::stringstream writeCallbackStream;
				bool writePtr = (var.m_PropertyFlags & PropertyFlags_SetCallback);

				if (writePtr)
				{
					writeCallbackStream << "WriteSetFunction(" << m_Class.m_Name << ", "
						<< var.m_Name << ", "
						<< var.GetType() << (var.IsAPointer() ? "*" : "") << ")";
				}
				else
				{
					writeCallbackStream << "0";
				}

				std::string simpleType;
				if (var.IsAPointer())
				{
					simpleType = "LXType_ObjectPtr";
				}
				else if ((var.GetType() == "bool") 
					  || (var.GetType() == "int") 
					  || (var.GetType() == "float") 
					  || (var.GetType() == "double") 
					  || (var.GetType().find("std::") != std::string::npos)
					  || (var.GetType().find("glm::") != std::string::npos))
				{
					std::string sanitizedType = RemoveSubstrings(var.GetType(), "::");
					simpleType = "LXType_" + sanitizedType;
				}
				else
				{
					simpleType = "LXType_Object";
				}

				std::string sizeProperty = "sizeof(" + var.GetType();
				
				if (!var.m_AssociatedType.empty())
				{
					sizeProperty += "<" + var.m_AssociatedType + (var.m_AssociatedPtr ? "*" : "") + ">";
				}
				if (var.IsAPointer())
				{
					sizeProperty += "*";
				}
				sizeProperty += ")";


				// warning! if you change anything here mirror it in property.h in LigumX
				WriteLine("{ \"" + varName + "\", "
					+ "PIDX_" + varName + ", "
					+ "offsetof(" + m_Class.m_Name + ", m_" + varName + "), "
					+ (m_Class.m_Members[i].GetType() == "\tbool" ? "1" : "0") + ", "
					+ simpleType + ", "
					+  sizeProperty+ ", "
					+ "LXType_" + RemoveSubstrings(varType, "::") + ", "
					+ (var.IsAPointer() ? "true" : "false") + ", "
					+ (var.m_IsTemplate ? ("LXType_" + var.m_AssociatedType) : "LXType_None") + ", "
					+ (var.m_AssociatedPtr ? "true" : "false") + ", "
					+ BuildPropertyFlagsString(var.m_PropertyFlags) + ", "
					+ var.GetMinValue() + ", "
					+ var.GetMaxValue() + ", "
					+ writeCallbackStream.str() + ","
					+ "}, ");
			}


			WriteLine("};");
		}
	}

	void WriteSerializer()
	{
		WriteLine("bool " + m_Class.m_Name + "::Serialize(Serializer2& serializer)");
		WriteLine("{");
		WriteLine("	return true;");
		WriteLine("}");

		WriteLine("bool " + m_Class.m_Name + "::Serialize(bool writing)");
		WriteLine("{");
		WriteLine(R"(	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); )");
		WriteLine(R"(	Serialize(serializer2); )");
		WriteLine("");
		WriteLine(R"(	bool success = g_Serializer->SerializeObject(this, writing); )");

		if (m_Class.m_PropertyFlags & ClassPropertyFlags_PostSerialization)
		{
			WriteLine("	PostSerialization(writing, success);");
		}
		WriteLine("	return success;");
		WriteLine("}");
	}

	void WriteGetTypeName()
	{
		WriteLine("const char* " + m_Class.m_Name + "::GetTypeName()");
		WriteLine("{");
		WriteLine("\treturn ClassName;");
		WriteLine("}");

	}

	enum OutputType
	{
		Imgui = 0,
		File
	};

	//struct OutputParams
	//{

	//	const char* m_PtrVector;
	//	const char* m_Vector;
	//	const char* m_Reference;
	//	const char* m_Reference;
	//};

	void WriteImguiVariable(Variable& var)
	{
		std::string prefix = "LXImgui_Show";
		std::string tabStop = "\t";

		std::string& varName = var.m_Name;
		std::string varValue = "m_" + varName;
		std::string extraArgs;
		std::string typeToWrite;

		const std::string& varType = var.GetType();

		bool write = false;

		if (var.m_IsPtr || var.m_IsTemplate)
		{
			// found
			write = true;

			if (var.m_IsTemplate)
			{
				if (var.m_AssociatedPtr)
				{
					typeToWrite = "ObjectPtrVector";
				}
				else
				{
					typeToWrite = "ObjectVector";
				}
			}
			else
			{
				typeToWrite = "ObjectPtr";
			}


			if (var.m_IsTemplate)
			{
				extraArgs = ", " + var.m_AssociatedType;
			}
			else
			{
				extraArgs = ", " + var.GetType();
			}


		}
		else
		{
			// not a ref to another object. maybe a primitive type? 
			auto findResult = g_LXTypeToImguiCallName.find(varType);

			if (findResult != g_LXTypeToImguiCallName.end())
			{
				// found
				write = true;

				std::string typeToWrite = findResult->second;

				if (VarTypeSupportsLimits(var))
				{
					// has limits
					extraArgs += ", " + var.GetMinValue() + ", " + var.GetMaxValue();
				}
				else
				{
				}

			}
		}

		if (var.m_PropertyFlags & PropertyFlags_SetCallback)
		{
			typeToWrite += "_SetCallback";
		}

		if (write)
		{
			WriteLine(tabStop + prefix + typeToWrite + "(\"" + varName + "\", " + varValue + ");");
		}
	}

	void WriteShowImgui()
	{
		WriteLine("bool " + m_Class.m_Name + "::ShowPropertyGrid()");
		WriteLine("{");


		if (!m_Class.IsLXObject())
		{
			WriteLine(tabStop + "super::ShowPropertyGrid();");
		}

		int numProperties = m_Class.m_Members.size();

		if (numProperties > 0)
		{
			for (int i = 0; i < numProperties; ++i)
			{
				Variable& var = m_Class.m_Members[i];
				
			}
		}
		WriteLine(tabStop + std::string("return true;"));
		WriteLine("}");
	}

	void WriteIncludes()
	{
		WriteLine("#include \"" + m_Class.m_Name + ".h\"");
		WriteLine("#include \"serializer.h\"");


		StringList neededIncludes;
		for (const Variable& var : m_Class.m_Members)
		{
			std::string varType;
			if (var.m_IsPtr)
			{
				varType = var.GetType();


			}
			else if (var.m_IsTemplate)
			{
				varType = var.m_AssociatedType;
			}

			if (!varType.empty())
			{
				auto findResult = std::find(neededIncludes.begin(), neededIncludes.end(), varType);

				if (findResult == neededIncludes.end())
				{
					neededIncludes.push_back(varType);
				}
			}
		}

		for (const std::string& include : neededIncludes)
		{
			WriteLine("#include \"" + include + ".h\"");
		}
	}



	void WriteBody()
	{
		WriteIncludes();

		
		WritePropertyArray();


		WriteSerializer();
		WriteShowImgui();
		WriteGetTypeName();
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
			WriteLine("const std::string EnumValues_" + anEnum.m_Name + "[] = ");
			WriteLine("{");
			for (std::string& name : anEnum.m_Values)
			{
				WriteLine("\"" + name + "\",");
			}
			WriteLine("};");

			WriteLine("");

			WriteLine("const " + anEnum.m_Name + " Indirection_" + anEnum.m_Name + "[] =");
			WriteLine("{");
			for (std::string& name : anEnum.m_Values)
			{
				WriteLine("	" + anEnum.m_Name + "_" + name + ",");
			}
			WriteLine("};");
		}
	}



	void Process()
	{
		if (!m_Class.m_IsValid)
		{
			WriteLine(m_CodeRegionBeginString);
			WriteEnums();
			WriteLine(m_CodeRegionEndString);
			WriteToFile();
			return;
		}

		// Write class header region
		WriteHeader();

		WriteBody();

		WriteEnums();

		WriteFooter();

		WriteToFile();
	}
};
