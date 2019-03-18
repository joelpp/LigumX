#include "OutputSerializer.h"

#pragma region  CLASS_SOURCE OutputSerializer

#include "OutputSerializer.h"
#include "serializer.h"
void OutputSerializer::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
}
bool OutputSerializer::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool OutputSerializer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	return true;
}
const char* OutputSerializer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE OutputSerializer

OutputSerializer::OutputSerializer(LXObject* object, bool writing, const std::string& fileName)
	: super(object, writing, fileName)
{

}
