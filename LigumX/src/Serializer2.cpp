#include "Serializer2.h"

#include "FileUtils.h"



#pragma region  CLASS_SOURCE Serializer2

#include "Serializer2.h"
#include "serializer.h"
#include "SerializerInputVariable.h"
const ClassPropertyData Serializer2::g_Properties[] = 
{
{ "Writing", PIDX_Writing, offsetof(Serializer2, m_Writing), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "IsValid", PIDX_IsValid, offsetof(Serializer2, m_IsValid), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Filename", PIDX_Filename, offsetof(Serializer2, m_Filename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "SerializerInputVariables", PIDX_SerializerInputVariables, offsetof(Serializer2, m_SerializerInputVariables), 0, LXType_stdvector, sizeof(std::vector<SerializerInputVariable>), LXType_stdvector, false, LXType_SerializerInputVariable, false, 0, 0, 0, 0,}, 
};
void Serializer2::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_Writing], m_Writing);
	serializer.SerializeBool(g_Properties[PIDX_IsValid], m_IsValid);
	serializer.SerializeString(g_Properties[PIDX_Filename], m_Filename);
	serializer.SerializeVector(g_Properties[PIDX_SerializerInputVariables], m_SerializerInputVariables);
}
bool Serializer2::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Serializer2::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("Writing", m_Writing  );
	ImguiHelpers::ShowBool("IsValid", m_IsValid  );
	ImguiHelpers::ShowString("Filename", m_Filename  );
	ImguiHelpers::ShowVector("SerializerInputVariables", m_SerializerInputVariables  );
	return true;
}
const char* Serializer2::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Serializer2

void Serializer2::ProcessFile()
{
	StringList lines = StringUtils::ReadFileLines(m_Filename);

	bool creatingVariable = false;
	bool readingValues = false;
	SerializerInputVariable currentVar;
	bool numberValue = false;

	bool addLastVar = true;
	for (const std::string& line : lines)
	{
		bool startsWithLetter = isalpha(line[0]) != 0 ? true : false;

		if (startsWithLetter)
		{
			if (!creatingVariable || numberValue)
			{
				if (numberValue)
				{
					AddTo_SerializerInputVariables(currentVar);
				}

				currentVar = SerializerInputVariable();  // only valid cause we assument no std::vexctor<std::string>
				currentVar.SetVariableName(line);

				creatingVariable = true;
				numberValue = false;
			}
			else
			{
				currentVar.AddTo_Values(line);

				AddTo_SerializerInputVariables(currentVar);

				creatingVariable = false;
			}
		}
		else
		{
			numberValue = true;
			currentVar.AddTo_Values(line);
		}
	}

	if (numberValue)
	{
		AddTo_SerializerInputVariables(currentVar);

	}

}



void Serializer2::Close()
{
	if (m_Writing)
	{
		StringUtils::DumpToFile(m_Filename, m_FileData);
	}
}



int FindVariableIndex(std::vector<SerializerInputVariable>& serializerInputVariables, const std::string& varName)
{
	int index = -1;
	for (int i = 0; i < serializerInputVariables.size(); ++i)
	{
		SerializerInputVariable& var = serializerInputVariables[i];
		if (var.GetVariableName() == varName)
		{
			return i;
		}
	}

	return index;
}


bool Serializer2::FindVariable(SerializerInputVariable& var, const std::string& varName)
{
	int index = FindVariableIndex(m_SerializerInputVariables, varName);

	if (index == -1)
	{
		return false;
	}
	else
	{
		var = m_SerializerInputVariables[index];

		auto it = m_SerializerInputVariables.begin() + index;
		m_SerializerInputVariables.erase(it);

		return true;
	}
}

bool Serializer2::SerializePropertyCommon(const ClassPropertyData& propertyData)
{
	if (propertyData.m_PropertyFlags & PropertyFlags_Transient)
	{
		return false;
	}
	
	m_FileData += propertyData.m_Name;
	m_FileData += gc_NewLine;

	return true;
}
///////////////////////////////

std::string Serializer2::GetAsSerialized(bool variable)
{
	std::stringstream sstr;
	sstr << variable ? "1" : "0";
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(float variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(int variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(lxInt64 variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(std::string& variable)
{
	std::stringstream sstr;
	sstr << variable;
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::ivec2& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::highp_ivec2& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::ivec3& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.z);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::vec2& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::vec3& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.z);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::ivec4& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.z);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.w);
	sstr << gc_NewLine;
	return sstr.str();
}

std::string Serializer2::GetAsSerialized(glm::vec4& variable)
{
	std::stringstream sstr;
	sstr << std::to_string(variable.x);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.y);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.z);
	sstr << gc_NewLine;
	sstr << std::to_string(variable.w);
	sstr << gc_NewLine;
	return sstr.str();
}


//////////////////////	

void Serializer2::SerializeBool(const ClassPropertyData& propertyData, bool& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += GetAsSerialized(variable);
		}
	}
	else
	{
		SerializeBool(propertyData.m_Name, variable);
	}
}


void Serializer2::SerializeFloat(const ClassPropertyData& propertyData, float& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeFloat(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeInt(const ClassPropertyData& propertyData, int& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeInt(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeInt64(const ClassPropertyData& propertyData, lxInt64& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeInt64(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeString(const ClassPropertyData& propertyData, std::string& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += variable;
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeString(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeIVec2(const ClassPropertyData& propertyData, glm::ivec2& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeIVec2(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeHighp_IVec2(const ClassPropertyData& propertyData, glm::highp_ivec2 & variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeHighp_IVec2(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeIVec3(const ClassPropertyData& propertyData, glm::ivec3& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.z);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeIVec3(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeVec2(const ClassPropertyData& propertyData, glm::vec2& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeVec2(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeVec3(const ClassPropertyData& propertyData, glm::vec3& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.z);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeVec3(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeIVec4(const ClassPropertyData& propertyData, glm::ivec4& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeIVec4(propertyData.m_Name, variable);
	}
}

void Serializer2::SerializeVec4(const ClassPropertyData& propertyData, glm::vec4& variable)
{
	if (m_Writing)
	{
		bool output = SerializePropertyCommon(propertyData);
		if (output)
		{
			m_FileData += std::to_string(variable.x);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.y);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.z);
			m_FileData += gc_NewLine;
			m_FileData += std::to_string(variable.w);
			m_FileData += gc_NewLine;
		}
	}
	else
	{
		SerializeVec4(propertyData.m_Name, variable);
	}
}



///////////////////////////////

void Serializer2::SerializeBool(const std::string& varName, bool& variable)
{
	if (m_Writing)
	{
		
	}
	else
	{
		FIND_VARIABLE_SIZE(1);

		int val = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable = (val == 1);
	}
}

void Serializer2::SerializeFloat(const std::string& varName, float& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(1);

		float val = StringUtils::ToFloat(serializerVariable.GetValues()[0]);
		variable = val;
	}
}

void Serializer2::SerializeInt(const std::string& varName, int& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		
		FIND_VARIABLE_SIZE(1);

		int val = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable = val;
	}
}

void Serializer2::SerializeInt64(const std::string& varName, lxInt64& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(1);

		lxInt64 val = StringUtils::ToInt64(serializerVariable.GetValues()[0]);
		variable = val;
	}
}

void Serializer2::SerializeString(const std::string& varName, std::string& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(1);

		variable = serializerVariable.GetValues()[0];
	}
}

void Serializer2::SerializeIVec2(const std::string& varName, glm::ivec2& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(2);

		variable.x = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToInt(serializerVariable.GetValues()[1]);
	}
}

void Serializer2::SerializeHighp_IVec2(const std::string& varName, glm::highp_ivec2& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(2);

		variable.x = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToInt(serializerVariable.GetValues()[1]);
	}
}

void Serializer2::SerializeIVec3(const std::string& varName, glm::ivec3& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(3);

		variable.x = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToInt(serializerVariable.GetValues()[1]);
		variable.z = StringUtils::ToInt(serializerVariable.GetValues()[2]);
	}
}

void Serializer2::SerializeVec2(const std::string& varName, glm::vec2& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(2);

		variable.x = StringUtils::ToFloat(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToFloat(serializerVariable.GetValues()[1]);
	}
}

void Serializer2::SerializeVec3(const std::string& varName, glm::vec3& variable)
{
	if (m_Writing)
	{
		//m_FileStream << variable.x << std::endl;
		//m_FileStream << variable.y << std::endl;
		//m_FileStream << variable.z << std::endl;
	}
	else
	{
		FIND_VARIABLE_SIZE(3);

		variable.x = StringUtils::ToFloat(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToFloat(serializerVariable.GetValues()[1]);
		variable.z = StringUtils::ToFloat(serializerVariable.GetValues()[2]);
	}

}

void Serializer2::SerializeIVec4(const std::string& varName, glm::ivec4& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(4);

		variable.x = StringUtils::ToInt(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToInt(serializerVariable.GetValues()[1]);
		variable.z = StringUtils::ToInt(serializerVariable.GetValues()[2]);
		variable.w = StringUtils::ToInt(serializerVariable.GetValues()[3]);
	}
}

void Serializer2::SerializeVec4(const std::string& varName, glm::vec4& variable)
{
	if (m_Writing)
	{

	}
	else
	{
		FIND_VARIABLE_SIZE(4);

		variable.x = StringUtils::ToFloat(serializerVariable.GetValues()[0]);
		variable.y = StringUtils::ToFloat(serializerVariable.GetValues()[1]);
		variable.z = StringUtils::ToFloat(serializerVariable.GetValues()[2]);
		variable.w = StringUtils::ToFloat(serializerVariable.GetValues()[3]);
	}
}


//////////////////////////////////////////
