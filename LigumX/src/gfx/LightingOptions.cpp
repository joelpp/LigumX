#include "LightingOptions.h"

#pragma region  CLASS_SOURCE LightingOptions

#include "LightingOptions.h"
#include "serializer.h"
const ClassPropertyData LightingOptions::g_Properties[] = 
{
{ "EnableDynamicLights", PIDX_EnableDynamicLights, offsetof(LightingOptions, m_EnableDynamicLights), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSunlight", PIDX_EnableSunlight, offsetof(LightingOptions, m_EnableSunlight), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableAmbientLighting", PIDX_EnableAmbientLighting, offsetof(LightingOptions, m_EnableAmbientLighting), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "AmbientLightingGlobalFactor", PIDX_AmbientLightingGlobalFactor, offsetof(LightingOptions, m_AmbientLightingGlobalFactor), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "EnableDiffuseComponent", PIDX_EnableDiffuseComponent, offsetof(LightingOptions, m_EnableDiffuseComponent), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSpecularComponent", PIDX_EnableSpecularComponent, offsetof(LightingOptions, m_EnableSpecularComponent), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableReflection", PIDX_EnableReflection, offsetof(LightingOptions, m_EnableReflection), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSunShadow", PIDX_EnableSunShadow, offsetof(LightingOptions, m_EnableSunShadow), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableDynamicShadows", PIDX_EnableDynamicShadows, offsetof(LightingOptions, m_EnableDynamicShadows), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableReplacementAlbedo", PIDX_EnableReplacementAlbedo, offsetof(LightingOptions, m_EnableReplacementAlbedo), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ReplacementAlbedo", PIDX_ReplacementAlbedo, offsetof(LightingOptions, m_ReplacementAlbedo), 0, LXType_glmvec3, sizeof(glm::vec3), LXType_glmvec3, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "EnableReplacementMetallic", PIDX_EnableReplacementMetallic, offsetof(LightingOptions, m_EnableReplacementMetallic), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ReplacementMetallic", PIDX_ReplacementMetallic, offsetof(LightingOptions, m_ReplacementMetallic), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "EnableReplacementRoughness", PIDX_EnableReplacementRoughness, offsetof(LightingOptions, m_EnableReplacementRoughness), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "ReplacementRoughness", PIDX_ReplacementRoughness, offsetof(LightingOptions, m_ReplacementRoughness), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
};
void LightingOptions::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_EnableDynamicLights], m_EnableDynamicLights);
	serializer.SerializeBool(g_Properties[PIDX_EnableSunlight], m_EnableSunlight);
	serializer.SerializeBool(g_Properties[PIDX_EnableAmbientLighting], m_EnableAmbientLighting);
	serializer.SerializeFloat(g_Properties[PIDX_AmbientLightingGlobalFactor], m_AmbientLightingGlobalFactor);
	serializer.SerializeBool(g_Properties[PIDX_EnableDiffuseComponent], m_EnableDiffuseComponent);
	serializer.SerializeBool(g_Properties[PIDX_EnableSpecularComponent], m_EnableSpecularComponent);
	serializer.SerializeBool(g_Properties[PIDX_EnableReflection], m_EnableReflection);
	serializer.SerializeBool(g_Properties[PIDX_EnableSunShadow], m_EnableSunShadow);
	serializer.SerializeBool(g_Properties[PIDX_EnableDynamicShadows], m_EnableDynamicShadows);
	serializer.SerializeBool(g_Properties[PIDX_EnableReplacementAlbedo], m_EnableReplacementAlbedo);
	serializer.SerializeVec3(g_Properties[PIDX_ReplacementAlbedo], m_ReplacementAlbedo);
	serializer.SerializeBool(g_Properties[PIDX_EnableReplacementMetallic], m_EnableReplacementMetallic);
	serializer.SerializeFloat(g_Properties[PIDX_ReplacementMetallic], m_ReplacementMetallic);
	serializer.SerializeBool(g_Properties[PIDX_EnableReplacementRoughness], m_EnableReplacementRoughness);
	serializer.SerializeFloat(g_Properties[PIDX_ReplacementRoughness], m_ReplacementRoughness);
}
bool LightingOptions::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool LightingOptions::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableDynamicLights], &m_EnableDynamicLights  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableSunlight], &m_EnableSunlight  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableAmbientLighting], &m_EnableAmbientLighting  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_AmbientLightingGlobalFactor], &m_AmbientLightingGlobalFactor , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableDiffuseComponent], &m_EnableDiffuseComponent  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableSpecularComponent], &m_EnableSpecularComponent  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableReflection], &m_EnableReflection  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableSunShadow], &m_EnableSunShadow  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableDynamicShadows], &m_EnableDynamicShadows  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableReplacementAlbedo], &m_EnableReplacementAlbedo  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ReplacementAlbedo], &m_ReplacementAlbedo , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableReplacementMetallic], &m_EnableReplacementMetallic  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ReplacementMetallic], &m_ReplacementMetallic , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableReplacementRoughness], &m_EnableReplacementRoughness  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_ReplacementRoughness], &m_ReplacementRoughness , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	return true;
}
void LightingOptions::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	LightingOptions* other = (LightingOptions*) otherObj;
	other->SetEnableDynamicLights(m_EnableDynamicLights);
	other->SetEnableSunlight(m_EnableSunlight);
	other->SetEnableAmbientLighting(m_EnableAmbientLighting);
	other->SetAmbientLightingGlobalFactor(m_AmbientLightingGlobalFactor);
	other->SetEnableDiffuseComponent(m_EnableDiffuseComponent);
	other->SetEnableSpecularComponent(m_EnableSpecularComponent);
	other->SetEnableReflection(m_EnableReflection);
	other->SetEnableSunShadow(m_EnableSunShadow);
	other->SetEnableDynamicShadows(m_EnableDynamicShadows);
	other->SetEnableReplacementAlbedo(m_EnableReplacementAlbedo);
	other->SetReplacementAlbedo(m_ReplacementAlbedo);
	other->SetEnableReplacementMetallic(m_EnableReplacementMetallic);
	other->SetReplacementMetallic(m_ReplacementMetallic);
	other->SetEnableReplacementRoughness(m_EnableReplacementRoughness);
	other->SetReplacementRoughness(m_ReplacementRoughness);
}
const char* LightingOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE LightingOptions
