#include "Serializer2.h"

#pragma region  CLASS_SOURCE Serializer2

#include "Serializer2.h"
#include "LXObject.h"
const ClassPropertyData Serializer2::g_Properties[] = 
{
{ "Writing", PIDX_Writing, offsetof(Serializer2, m_Writing), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "IsValid", PIDX_IsValid, offsetof(Serializer2, m_IsValid), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Filename", PIDX_Filename, offsetof(Serializer2, m_Filename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Object", PIDX_Object, offsetof(Serializer2, m_Object), 0, LXType_ObjectPtr, sizeof(LXObject*), LXType_LXObject, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool Serializer2::Serialize(bool writing)
{
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
