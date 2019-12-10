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
	serializer.SerializeBool("Writing", m_Writing);
	serializer.SerializeBool("IsValid", m_IsValid);
	serializer.SerializeString("Filename", m_Filename);
	serializer.SerializeVector("SerializerInputVariables", m_SerializerInputVariables);
}
bool Serializer2::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

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
