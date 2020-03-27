#include "TestComponentA.h"

#pragma region  CLASS_SOURCE TestComponentA

#include "TestComponentA.h"
#include "serializer.h"
void TestComponentA::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
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
	return true;
}
const char* TestComponentA::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE TestComponentA
