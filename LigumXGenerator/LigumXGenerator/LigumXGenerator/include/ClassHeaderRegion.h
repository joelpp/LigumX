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


	bool GenerateFileStub(std::string& stub)
	{
		std::stringstream ss;

		ss << "#pragma once";
		ss << "\n";
		ss << "\n";
		ss << "\n";


		ss << "\n";
		ss << "\n";
		ss << "};";

		stub = ss.str();
		return true;
	}

	bool IsBeginMarker(TokenList& token, bool createStub)
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
		WriteLine(m_CodeRegionBeginString);
		Write("class ");
		Write(m_Class.m_Name);

		if (!(m_Class.IsLXObject()))
		{
			Write(" : public ");
			Write(m_Class.m_ParentName);
		}

		Write("\n");
		WriteLine("{");
		WriteLine("public:");
		m_Stream << "static const int ClassID = " << std::hash_value(m_Class.m_Name) << ";" << std::endl;


		m_Stream << "static const LXType Type = LXType_" << m_Class.m_Name << ";" << std::endl;
		m_Stream << "static constexpr const char* ClassName = \"" << m_Class.m_Name << "\";" << std::endl;


		if (!(m_Class.IsLXObject()))
		{
			WriteLine("typedef " + m_Class.m_ParentName + " super;");
		}


		m_Stream << std::endl;
	}

	void WriteFooter()
	{
		m_Stream << std::endl;
		m_Stream << m_CodeRegionEndString;
	}

	void WriteClassVariableGettersAndSetters()
	{
		for (Variable& var : m_Class.m_Members)
		{
			// Write getter
			bool ptr = var.IsAPointer();
			bool vector = var.m_IsTemplate;
			bool glmObject = var.IsGLMType();
			bool isString = var.IsString();

			bool isPrimitive = var.IsPrimitive();

			std::stringstream getterStream;

			std::stringstream returnTypeStream;

			returnTypeStream << ((ptr || vector || glmObject || isPrimitive) ? "" : "const ");
			returnTypeStream << (var.GetType());

			if (var.m_IsTemplate)
			{
				returnTypeStream << "<";
				returnTypeStream << var.m_AssociatedType << (var.m_AssociatedPtr ? "*" : "");
				returnTypeStream << ">";
			}

			if (ptr)
			{
				returnTypeStream << "*&";
			}
			else if (!isPrimitive)
			{
				returnTypeStream << "&";
			}

			returnTypeStream << (" ");
			getterStream << "Get" << var.m_Name << "() { ";

			if (var.m_PropertyFlags & PropertyFlags_GetCallback)
			{
				getterStream << "return Get" << var.m_Name << "_Callback(); }";
			}
			else
			{
				getterStream << "return m_" << var.m_Name << "; }; ";
			}

			getterStream << std::endl;

			std::string getterString = returnTypeStream.str() + getterStream.str();
			m_Stream << getterString;
			std::stringstream callbackFuncStream;
			if (var.m_PropertyFlags & PropertyFlags_GetCallback)
			{
				callbackFuncStream << "Get" << var.m_Name << "_Callback();";
				callbackFuncStream << std::endl;
				std::string callbackFuncString = returnTypeStream.str() + callbackFuncStream.str();
				m_Stream << callbackFuncString;
			}



			// Write setter
			m_Stream << "void Set" << var.m_Name << "(";
				
			m_Stream << (glmObject ? "const " : "");

			m_Stream << var.GetType();

			if (var.m_IsTemplate)
			{
				m_Stream << "<";
				m_Stream << var.m_AssociatedType << (var.m_AssociatedPtr ? "*" : "");
				m_Stream << ">";
			}

			m_Stream << (ptr ? "*" : "");
			m_Stream << (glmObject ? "&" : "");
			m_Stream << " value) { ";

			if (var.m_PropertyFlags & PropertyFlags_SetCallback)
			{
				m_Stream << "Set" << var.m_Name << "_Callback(value);";
			}
			else
			{
				m_Stream << "m_" << var.m_Name << " = value;";
			}

			m_Stream << " }; ";

			m_Stream << std::endl;

			if (var.m_PropertyFlags & PropertyFlags_SetCallback)
			{
				m_Stream << "void Set" << var.m_Name << "_Callback(";
				m_Stream << (ptr ? "" : "const ");
				m_Stream << var.GetType();
				m_Stream << (ptr ? "*" : "&");
				m_Stream << " value);";
				m_Stream << std::endl;
			}

			if (var.m_PropertyFlags & PropertyFlags_Adder)
			{
				m_Stream << "void AddTo_" << var.m_Name << "(" << var.GetType();
				m_Stream << " value) { m_" << var.m_Name << " += value; };";
				m_Stream << std::endl;
			}

			if (vector)
			{
				m_Stream << "void AddTo_" << var.m_Name << "(" << var.m_AssociatedType << (var.m_AssociatedPtr ? "*" : "");
				m_Stream << " value) { m_" << var.m_Name << ".push_back(value); };";
				m_Stream << std::endl;
			}

		}
	}

	void WriteProperties()
	{
		WriteLine("public:");

		// TODO : functions to compute this or store it
		std::string propertyCountVarName = "g_PropertyCount";
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

	void WritePropertyIndexEnum()
	{
		WriteLine("enum g_" + m_Class.m_Name + "PIDX");
		WriteLine("{");

		for (Variable var : m_Class.m_Members)
		{
			WriteLine("PIDX_" + var.m_Name + ",");
		}
		WriteLine("};");
	}

	void WriteSerializerDeclaration()
	{
		WriteLine("bool Serialize(bool writing);");

		if (m_Class.m_PropertyFlags & ClassPropertyFlags_PostSerialization)
		{
			WriteLine("void PostSerialization(bool writing, bool success);");
		}
	}

	void WriteShowImguiDeclaration()
	{
		WriteLine("virtual bool ShowPropertyGrid();");
	}

	void WriteGetTypeNameDeclaration()
	{
		WriteLine("virtual const char* GetTypeName();");
	}

	void WriteBody()
	{
		WriteClassVariableGettersAndSetters();

		std::cout << m_Class.m_Name << std::endl;
		m_Stream << "private:" << std::endl;
		for (const Variable& var : m_Class.m_Members)
		{
			m_Stream << var.GetType();

			if (var.m_IsTemplate)
			{
				m_Stream << "<";
				m_Stream << var.m_AssociatedType << (var.m_AssociatedPtr ? "*" : "");
				m_Stream << ">";
			}

			m_Stream << (var.IsAPointer() ? "*" : "");
			m_Stream << " m_" << var.m_Name;

			if (var.HasDefaultValue())
			{
				m_Stream << " = " << var.m_DefaultValue;
			}

			m_Stream << ";";
			m_Stream << std::endl;
		}

		WriteProperties();
		
		WriteDefaultConstructor();

		WritePropertyIndexEnum();

		WriteSerializerDeclaration();

		WriteShowImguiDeclaration();
		WriteGetTypeNameDeclaration();
	}



	void Process()
	{
		if (!m_Class.m_IsValid)
		{
			return;
		}

		// Write class header region
		WriteHeader();


		WriteBody();


		WriteFooter();

		WriteToFile();
	}
};