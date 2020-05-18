
#include "glm\glm.hpp"

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include <cstddef>

#pragma region  CLASS_SOURCE SunLight

#include "SunLight.h"
#include "serializer.h"
#include "Texture.h"
const ClassPropertyData SunLight::g_Properties[] = 
{
{ "UseShadowMap", PIDX_UseShadowMap, offsetof(SunLight, m_UseShadowMap), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Time", PIDX_Time, offsetof(SunLight, m_Time), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Orientation", PIDX_Orientation, offsetof(SunLight, m_Orientation), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Speed", PIDX_Speed, offsetof(SunLight, m_Speed), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "UseSkybox", PIDX_UseSkybox, offsetof(SunLight, m_UseSkybox), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(SunLight, UseSkybox, bool),}, 
{ "Skybox", PIDX_Skybox, offsetof(SunLight, m_Skybox), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, PropertyFlags_Hidden, 0, 0, 0,}, 
};
void SunLight::Serialize(Serializer2& serializer)
{
	super::Serialize(serializer);
	serializer.SerializeBool(g_Properties[PIDX_UseShadowMap], m_UseShadowMap);
	serializer.SerializeFloat(g_Properties[PIDX_Time], m_Time);
	serializer.SerializeFloat(g_Properties[PIDX_Orientation], m_Orientation);
	serializer.SerializeFloat(g_Properties[PIDX_Speed], m_Speed);
	serializer.SerializeBool(g_Properties[PIDX_UseSkybox], m_UseSkybox);
	serializer.SerializeObjectPtr(g_Properties[PIDX_Skybox], m_Skybox);
}
bool SunLight::Serialize(bool writing)
{
	Serializer2 serializer2 = Serializer2::CreateSerializer(this, writing); 
	Serialize(serializer2); 
	serializer2.Close();

	bool success = true;//g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SunLight::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseShadowMap], &m_UseShadowMap  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Time], &m_Time , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Orientation], &m_Orientation , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Speed], &m_Speed , LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_UseSkybox], &m_UseSkybox  );
	ImguiHelpers::ShowProperty(this, g_Properties[PIDX_Skybox], m_Skybox  );
	return true;
}
const char* SunLight::GetTypeName()
{
	return ClassName;
}

#pragma endregion  CLASS_SOURCE SunLight
SunLight::SunLight()
{
	SetObjectID(g_ObjectManager->GetNewObjectID());
}

glm::vec3 SunLight::GetSunDirection()
{
	glm::vec2 sunDirFlat = glm::vec2(cos(m_Orientation), sin(m_Orientation));
	return cos(m_Time) * glm::vec3(0, 0, 1) + sin(m_Time) * glm::vec3(sunDirFlat.x, sunDirFlat.y, 0);
}

void SunLight::SetUseSkybox_Callback(const bool& value )
{
	m_UseSkybox = value;
}
