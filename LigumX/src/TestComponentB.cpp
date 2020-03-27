#include "TestComponentB.h"

#pragma region  CLASS_SOURCE TestComponentB

#include "TestComponentB.h"
#include "serializer.h"
void TestComponentB::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
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
	return true;
}
const char* TestComponentB::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE TestComponentB
