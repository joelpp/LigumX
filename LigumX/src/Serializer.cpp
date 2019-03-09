#include "Serializer.h"

#pragma region  CLASS_SOURCE Serializer

#include "Serializer.h"
const ClassPropertyData Serializer::g_Properties[] = 
{
{ "writing", PIDX_writing, offsetof(Serializer, m_writing), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "IsValid", PIDX_IsValid, offsetof(Serializer, m_IsValid), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Filename", PIDX_Filename, offsetof(Serializer, m_Filename), 0, LXType_stdstring, sizeof(std::string), LXType_stdstring, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool Serializer::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool Serializer::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("writing", m_writing);
	LXIMGUI_SHOW_BOOL("IsValid", m_IsValid);
	LXIMGUI_SHOW_STRING("Filename", m_Filename);
	return true;
}
const char* Serializer::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE Serializer
