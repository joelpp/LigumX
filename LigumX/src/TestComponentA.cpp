#include "TestComponentA.h"

#pragma region  CLASS_SOURCE TestComponentA

#include "TestComponentA.h"
#include "serializer.h"
const ClassPropertyData TestComponentA::g_Properties[] = 
{
{ "AValue", PIDX_AValue, offsetof(TestComponentA, m_AValue), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void TestComponentA::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_AValue], m_AValue);
}
bool TestComponentA::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool TestComponentA::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_AValue], &m_AValue  );
	return true;
}
const char* TestComponentA::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE TestComponentA
