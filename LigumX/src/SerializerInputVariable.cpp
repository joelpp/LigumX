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
}
bool SerializerInputVariable::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SerializerInputVariable::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_VariableName], m_VariableName  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Values], m_Values  );
	return true;
}
void SerializerInputVariable::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	SerializerInputVariable* other = (SerializerInputVariable*) otherObj;
	other->SetVariableName(m_VariableName);
	other->SetValues(m_Values);
}
const char* SerializerInputVariable::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE SerializerInputVariable
