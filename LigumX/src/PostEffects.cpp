#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects
#include "PostEffects.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData PostEffects::g_Properties[] = 
{
{ "ObjectID", offsetof(PostEffects, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "GammaCorrectionEnabled", offsetof(PostEffects, m_GammaCorrectionEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "GammaExponent", offsetof(PostEffects, m_GammaExponent), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "ToneMappingEnabled", offsetof(PostEffects, m_ToneMappingEnabled), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
};
void PostEffects::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE PostEffects

PostEffects::PostEffects()
{
	m_ObjectID = rand();
}
