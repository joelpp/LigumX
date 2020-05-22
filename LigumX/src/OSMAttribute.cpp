#include "OSMAttribute.h"

#pragma region  CLASS_SOURCE OSMAttribute

#include "OSMAttribute.h"
#include "serializer.h"
const ClassPropertyData OSMAttribute::g_Properties[] = 
{
{ "Key", PIDX_Key, offsetof(OSMAttribute, m_Key), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Value", PIDX_Value, offsetof(OSMAttribute, m_Value), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void OSMAttribute::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeString(g_Properties[PIDX_Key], m_Key);
	serializer.SerializeString(g_Properties[PIDX_Value], m_Value);
}
bool OSMAttribute::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OSMAttribute::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_Key], m_Key  );
	ImguiHelpers::ShowString2(this, g_Properties[PIDX_Value], m_Value  );
	return true;
}
const char* OSMAttribute::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OSMAttribute


OSMAttribute::OSMAttribute(const std::string& _key, const std::string& _value)
	: m_Key(_key)
	, m_Value(_value)
{

}
