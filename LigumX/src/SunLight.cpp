#include "stdafx.h"
#include "glm\glm.hpp"

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include <cstddef>

#pragma region  CLASS_SOURCE SunLight

#include "SunLight.h"
#include "Texture.h"
const ClassPropertyData SunLight::g_Properties[] = 
{
{ "UseShadowMap", PIDX_UseShadowMap, offsetof(SunLight, m_UseShadowMap), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Time", PIDX_Time, offsetof(SunLight, m_Time), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Orientation", PIDX_Orientation, offsetof(SunLight, m_Orientation), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "Speed", PIDX_Speed, offsetof(SunLight, m_Speed), 0, LXType_float, sizeof(float), LXType_float, false, LXType_None, false, 0, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX, 0,}, 
{ "UseSkybox", PIDX_UseSkybox, offsetof(SunLight, m_UseSkybox), 0, LXType_bool, sizeof(bool), LXType_bool, false, LXType_None, false, 0, 0, 0, 0,}, 
{ "Skybox", PIDX_Skybox, offsetof(SunLight, m_Skybox), 0, LXType_ObjectPtr, sizeof(Texture*), LXType_Texture, true, LXType_None, false, PropertyFlags_Hidden, 0, 0, 0,}, 
};
bool SunLight::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}
bool SunLight::ShowPropertyGrid()
{
	super::ShowPropertyGrid();
	LXIMGUI_SHOW_BOOL("UseShadowMap", m_UseShadowMap);
	LXIMGUI_SHOW_FLOAT("Time", m_Time, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("Orientation", m_Orientation, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_FLOAT("Speed", m_Speed, LX_LIMITS_FLOAT_MIN, LX_LIMITS_FLOAT_MAX);
	LXIMGUI_SHOW_BOOL("UseSkybox", m_UseSkybox);
	LXIMGUI_SHOW_OBJECTREF("Skybox", m_Skybox);
	return true;
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
