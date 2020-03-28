#include "TestComponentB.h"

#pragma region  CLASS_SOURCE TestComponentB

#include "TestComponentB.h"
#include "serializer.h"
const ClassPropertyData TestComponentB::g_Properties[] = 
{
{ "BValueLol", PIDX_BValueLol, offsetof(TestComponentB, m_BValueLol), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void TestComponentB::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_BValueLol], m_BValueLol);
}
bool TestComponentB::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool TestComponentB::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_BValueLol], &m_BValueLol  );
	return true;
}
const char* TestComponentB::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE TestComponentB
