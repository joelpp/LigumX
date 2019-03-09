#include "Serializer2.h"

#pragma region  CLASS_SOURCE Serializer2

#include "Serializer2.h"
#include "serializer.h"
#include "LXObject.h"
const ClassPropertyData Serializer2::g_Properties[] = 
{
{ "Writing", PIDX_Writing, offsetof(Serializer2, m_Writing), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "IsValid", PIDX_IsValid, offsetof(Serializer2, m_IsValid), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Filename", PIDX_Filename, offsetof(Serializer2, m_Filename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Object", PIDX_Object, offsetof(Serializer2, m_Object), 0, LXType_ObjectPtr, sizeof(LXObject*), LXType_LXObject, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool Serializer2::Serialize(Serializer2& serializer)
{
	return true;
}
bool Serializer2::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Serializer2::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("Writing", m_Writing);
	LXIMGUI_SHOW_BOOL("IsValid", m_IsValid);
	LXIMGUI_SHOW_STRING("Filename", m_Filename);
	LXIMGUI_SHOW_OBJECTREF("Object", m_Object);
	return true;
}
const char* Serializer2::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Serializer2


Serializer2::Serializer2(LXObject* object, bool writing, const std::string& fileName)
	: m_Writing(writing)
	, m_Filename(fileName)
	, m_FileStream(fileName, (writing ? std::fstream::out : std::fstream::in))
{
	bool hardcodedId = g_ObjectManager->IsHardcodedID(object->GetObjectID());

	m_IsValid = !(m_Writing && hardcodedId);

	m_IsValid &= m_FileStream.is_open();
}

void Serializer2::Close()
{
	m_FileStream.close();
}

Serializer2 Serializer2::CreateSerializer(LXObject* object, bool writing)
{
	std::string basePath = g_PathObjects;
	std::string fileName = object->ClassName + std::string("_") + std::to_string(object->GetObjectID()) + ".LXobj";
	std::string fullPath = basePath + fileName;

	return Serializer2(object, writing, fileName);
}
