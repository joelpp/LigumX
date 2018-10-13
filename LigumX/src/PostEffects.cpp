#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects

#include "PostEffects.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData PostEffects::g_Properties[] = 
{
{ "GammaCorrectionEnabled", PIDX_GammaCorrectionEnabled, offsetof(PostEffects, m_GammaCorrectionEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "GammaExponent", PIDX_GammaExponent, offsetof(PostEffects, m_GammaExponent), 0, LXType_float, false, LXType_None, 0, 0, 0, 0,}, 
{ "ToneMappingEnabled", PIDX_ToneMappingEnabled, offsetof(PostEffects, m_ToneMappingEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "EmissiveGlowEnabled", PIDX_EmissiveGlowEnabled, offsetof(PostEffects, m_EmissiveGlowEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
};
bool PostEffects::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE PostEffects

PostEffects::PostEffects()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}
