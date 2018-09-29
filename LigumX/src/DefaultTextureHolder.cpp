#include "DefaultTextureHolder.h"
#include "Texture.h"

#pragma region  CLASS_SOURCE DefaultTextureHolder
DefaultTextureHolder* g_DefaultTextureHolder;

#include "DefaultTextureHolder.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData DefaultTextureHolder::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(DefaultTextureHolder, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(DefaultTextureHolder, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "GrassTexture", PIDX_GrassTexture, offsetof(DefaultTextureHolder, m_GrassTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, 0,}, 
{ "RockTexture", PIDX_RockTexture, offsetof(DefaultTextureHolder, m_RockTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, 0,}, 
{ "WoodTexture", PIDX_WoodTexture, offsetof(DefaultTextureHolder, m_WoodTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, 0,}, 
{ "SandTexture", PIDX_SandTexture, offsetof(DefaultTextureHolder, m_SandTexture), 0, LXType_Texture, true, LXType_None, 0, 0, 0, 0,}, 
};
bool DefaultTextureHolder::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE DefaultTextureHolder

DefaultTextureHolder::DefaultTextureHolder()
{
	m_GrassTexture = new Texture(10691);
	m_RockTexture = new Texture(10692);
	m_SandTexture = new Texture(10693);
	m_WoodTexture = new Texture(11039);
}
