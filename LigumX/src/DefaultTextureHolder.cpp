#include "DefaultTextureHolder.h"
#include "Texture.h"

#pragma region  CLASS_SOURCE DefaultTextureHolder
DefaultTextureHolder* g_DefaultTextureHolder;

#include "DefaultTextureHolder.h"
#include "serializer.h"
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
void DefaultTextureHolder::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeObjectPtr(g_Properties[PIDX_AsphaltTexture], m_AsphaltTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_DirtTexture], m_DirtTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_GrassTexture], m_GrassTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_RockTexture], m_RockTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_WoodTexture], m_WoodTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_SandTexture], m_SandTexture);
	serializer.SerializeObjectPtr(g_Properties[PIDX_WaterTexture], m_WaterTexture);
}
bool DefaultTextureHolder::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool DefaultTextureHolder::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowObjectPtr("AsphaltTexture", m_AsphaltTexture  );
	ImguiHelpers::ShowObjectPtr("DirtTexture", m_DirtTexture  );
	ImguiHelpers::ShowObjectPtr("GrassTexture", m_GrassTexture  );
	ImguiHelpers::ShowObjectPtr("RockTexture", m_RockTexture  );
	ImguiHelpers::ShowObjectPtr("WoodTexture", m_WoodTexture  );
	ImguiHelpers::ShowObjectPtr("SandTexture", m_SandTexture  );
	ImguiHelpers::ShowObjectPtr("WaterTexture", m_WaterTexture  );
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
