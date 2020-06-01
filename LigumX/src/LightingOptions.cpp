#include "LightingOptions.h"

#pragma region  CLASS_SOURCE LightingOptions

#include "LightingOptions.h"
#include "serializer.h"
const ClassPropertyData LightingOptions::g_Properties[] = 
{
{ "EnableDynamicLights", PIDX_EnableDynamicLights, offsetof(LightingOptions, m_EnableDynamicLights), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSunlight", PIDX_EnableSunlight, offsetof(LightingOptions, m_EnableSunlight), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableAmbientLighting", PIDX_EnableAmbientLighting, offsetof(LightingOptions, m_EnableAmbientLighting), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableDiffuseComponent", PIDX_EnableDiffuseComponent, offsetof(LightingOptions, m_EnableDiffuseComponent), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSpecularComponent", PIDX_EnableSpecularComponent, offsetof(LightingOptions, m_EnableSpecularComponent), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableReflection", PIDX_EnableReflection, offsetof(LightingOptions, m_EnableReflection), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableSunShadow", PIDX_EnableSunShadow, offsetof(LightingOptions, m_EnableSunShadow), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "EnableDynamicShadows", PIDX_EnableDynamicShadows, offsetof(LightingOptions, m_EnableDynamicShadows), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
};
void LightingOptions::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_EnableDynamicLights], m_EnableDynamicLights);
	serializer.SerializeBool(g_Properties[PIDX_EnableSunlight], m_EnableSunlight);
	serializer.SerializeBool(g_Properties[PIDX_EnableAmbientLighting], m_EnableAmbientLighting);
	serializer.SerializeBool(g_Properties[PIDX_EnableDiffuseComponent], m_EnableDiffuseComponent);
	serializer.SerializeBool(g_Properties[PIDX_EnableSpecularComponent], m_EnableSpecularComponent);
	serializer.SerializeBool(g_Properties[PIDX_EnableReflection], m_EnableReflection);
	serializer.SerializeBool(g_Properties[PIDX_EnableSunShadow], m_EnableSunShadow);
	serializer.SerializeBool(g_Properties[PIDX_EnableDynamicShadows], m_EnableDynamicShadows);
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
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableDiffuseComponent], &m_EnableDiffuseComponent  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableSpecularComponent], &m_EnableSpecularComponent  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableReflection], &m_EnableReflection  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableSunShadow], &m_EnableSunShadow  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_EnableDynamicShadows], &m_EnableDynamicShadows  );
	return true;
}
void LightingOptions::Clone(LXObject* otherObj)
{
	super::Clone(otherObj);
	LightingOptions* other = (LightingOptions*) otherObj;
	other->SetEnableDynamicLights(m_EnableDynamicLights);
	other->SetEnableSunlight(m_EnableSunlight);
	other->SetEnableAmbientLighting(m_EnableAmbientLighting);
	other->SetEnableDiffuseComponent(m_EnableDiffuseComponent);
	other->SetEnableSpecularComponent(m_EnableSpecularComponent);
	other->SetEnableReflection(m_EnableReflection);
	other->SetEnableSunShadow(m_EnableSunShadow);
	other->SetEnableDynamicShadows(m_EnableDynamicShadows);
}
const char* LightingOptions::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE LightingOptions
