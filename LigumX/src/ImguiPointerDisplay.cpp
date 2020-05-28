#include "ImguiPointerDisplay.h"

#pragma region  CLASS_SOURCE ImguiPointerDisplay

#include "ImguiPointerDisplay.h"
#include "serializer.h"
#include "LXObject.h"
const ClassPropertyData ImguiPointerDisplay::g_Properties[] = 
{
{ "CurrentID", PIDX_CurrentID, offsetof(ImguiPointerDisplay, m_CurrentID), 0, LXType_int, sizeof(int), LXType_int, false, LXType_None, false, 0, (float)LX_LIMITS_INT_MIN, (float)LX_LIMITS_INT_MAX, 0,}, 
{ "DataPointer", PIDX_DataPointer, offsetof(ImguiPointerDisplay, m_DataPointer), 0, LXType_ObjectPtr, sizeof(LXObject*), LXType_LXObject, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "WasUsedThisFrame", PIDX_WasUsedThisFrame, offsetof(ImguiPointerDisplay, m_WasUsedThisFrame), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "OpenPopup", PIDX_OpenPopup, offsetof(ImguiPointerDisplay, m_OpenPopup), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void ImguiPointerDisplay::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeInt(g_Properties[PIDX_CurrentID], m_CurrentID);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DataPointer], m_DataPointer);
	serializer.SerializeBool(g_Properties[PIDX_WasUsedThisFrame], m_WasUsedThisFrame);
	serializer.SerializeBool(g_Properties[PIDX_OpenPopup], m_OpenPopup);
}
bool ImguiPointerDisplay::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool ImguiPointerDisplay::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_CurrentID], &m_CurrentID , LX_LIMITS_INT_MIN, LX_LIMITS_INT_MAX );
	ImguiHelpers::ShowObject2(this, g_Properties[PIDX_DataPointer], &m_DataPointer  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_WasUsedThisFrame], &m_WasUsedThisFrame  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_OpenPopup], &m_OpenPopup  );
	return true;
}
void ImguiPointerDisplay::Clone(ImguiPointerDisplay& other)
{
	super::Clone(other);
	other.SetCurrentID(m_CurrentID);
	other.SetDataPointer(m_DataPointer);
	other.SetWasUsedThisFrame(m_WasUsedThisFrame);
	other.SetOpenPopup(m_OpenPopup);
}
const char* ImguiPointerDisplay::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE ImguiPointerDisplay
