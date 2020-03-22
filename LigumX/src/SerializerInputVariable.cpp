#include "SerializerInputVariable.h"

#pragma region  CLASS_SOURCE SerializerInputVariable

#include "SerializerInputVariable.h"
#include "serializer.h"
const ClassPropertyData SerializerInputVariable::g_Properties[] = 
{
{ "VariableName", PIDX_VariableName, offsetof(SerializerInputVariable, m_VariableName), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Values", PIDX_Values, offsetof(SerializerInputVariable, m_Values), 0, LXType_stdvector, sizeof(std::vector<std::string>), LXType_stdvector, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void SerializerInputVariable::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeString(g_Properties[PIDX_VariableName], m_VariableName);
	serializer.SerializeVector(g_Properties[PIDX_Values], m_Values);
	serializer.Close();
}
bool SerializerInputVariable::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SerializerInputVariable::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowString("VariableName", m_VariableName  );
	ImguiHelpers::ShowVector("Values", m_Values  );
	return true;
}
const char* SerializerInputVariable::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE SerializerInputVariable
