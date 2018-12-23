#include "DefaultTextureHolder.h"
#include "Texture.h"

#pragma region  CLASS_SOURCE DefaultTextureHolder
DefaultTextureHolder* g_DefaultTextureHolder;

#include "DefaultTextureHolder.h"
#include "Texture.h"
const ClassPropertyData DefaultTextureHolder::g_Properties[] = 
{
{ "AsphaltTexture", PIDX_AsphaltTexture, offsetof(DefaultTextureHolder, m_AsphaltTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "DirtTexture", PIDX_DirtTexture, offsetof(DefaultTextureHolder, m_DirtTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "GrassTexture", PIDX_GrassTexture, offsetof(DefaultTextureHolder, m_GrassTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "RockTexture", PIDX_RockTexture, offsetof(DefaultTextureHolder, m_RockTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "WoodTexture", PIDX_WoodTexture, offsetof(DefaultTextureHolder, m_WoodTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "SandTexture", PIDX_SandTexture, offsetof(DefaultTextureHolder, m_SandTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
{ "WaterTexture", PIDX_WaterTexture, offsetof(DefaultTextureHolder, m_WaterTexture), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, 0, 0, 0, 0,}, 
};
bool DefaultTextureHolder::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DefaultTextureHolder::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_OBJECTREF("AsphaltTexture", m_AsphaltTexture);
	LXIMGUI_SHOW_OBJECTREF("DirtTexture", m_DirtTexture);
	LXIMGUI_SHOW_OBJECTREF("GrassTexture", m_GrassTexture);
	LXIMGUI_SHOW_OBJECTREF("RockTexture", m_RockTexture);
	LXIMGUI_SHOW_OBJECTREF("WoodTexture", m_WoodTexture);
	LXIMGUI_SHOW_OBJECTREF("SandTexture", m_SandTexture);
	LXIMGUI_SHOW_OBJECTREF("WaterTexture", m_WaterTexture);
	return true;
}
const char* DefaultTextureHolder::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE DefaultTextureHolder

DefaultTextureHolder::DefaultTextureHolder()
{
	m_AsphaltTexture = new Texture(95736);
	m_DirtTexture = new Texture(239767);
	m_WaterTexture = new Texture(7256);
	m_GrassTexture = new Texture(10691);
	m_RockTexture = new Texture(10692);
	m_SandTexture = new Texture(10693);
	m_WoodTexture = new Texture(11039);
}
