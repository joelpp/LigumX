#include "stdafx.h"

#pragma region  CLASS_SOURCE PostEffects

#include "PostEffects.h"
#include "serializer.h"
const ClassPropertyData PostEffects::g_Properties[] = 
{
{ "GammaCorrectionEnabled", PIDX_GammaCorrectionEnabled, offsetof(PostEffects, m_GammaCorrectionEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "GammaExponent", PIDX_GammaExponent, offsetof(PostEffects, m_GammaExponent), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "ToneMappingEnabled", PIDX_ToneMappingEnabled, offsetof(PostEffects, m_ToneMappingEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EmissiveGlowEnabled", PIDX_EmissiveGlowEnabled, offsetof(PostEffects, m_EmissiveGlowEnabled), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void PostEffects::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool("GammaCorrectionEnabled", m_GammaCorrectionEnabled);
	serializer.SerializeFloat("GammaExponent", m_GammaExponent);
	serializer.SerializeBool("ToneMappingEnabled", m_ToneMappingEnabled);
	serializer.SerializeBool("EmissiveGlowEnabled", m_EmissiveGlowEnabled);
}
bool PostEffects::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool PostEffects::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowBool("GammaCorrectionEnabled", m_GammaCorrectionEnabled  );
	ImguiHelpers::ShowFloat("GammaExponent", m_GammaExponent , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowBool("ToneMappingEnabled", m_ToneMappingEnabled  );
	ImguiHelpers::ShowBool("EmissiveGlowEnabled", m_EmissiveGlowEnabled  );
	return true;
}
const char* PostEffects::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE PostEffects

PostEffects::PostEffects()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}
