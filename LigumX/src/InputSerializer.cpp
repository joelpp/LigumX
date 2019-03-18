#include "InputSerializer.h"

#pragma region  CLASS_SOURCE InputSerializer

#include "InputSerializer.h"
#include "serializer.h"
void InputSerializer::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
}
bool InputSerializer::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool InputSerializer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	return true;
}
const char* InputSerializer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE InputSerializer


InputSerializer::InputSerializer(LXObject* object, bool writing, const std::string& fileName)
	: super(object, writing, fileName)
{

}
