#include "DebugRenderer.h"

#pragma region  CLASS_SOURCE DebugRenderer
DebugRenderer* g_DebugRenderer;

#include "DebugRenderer.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData DebugRenderer::g_Properties[] = 
{
{ "Enabled", PIDX_Enabled, offsetof(DebugRenderer, m_Enabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
bool DebugRenderer::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE DebugRenderer
