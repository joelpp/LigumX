#include "stdafx.h"
#include "glm\glm.hpp"

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include <cstddef>

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectIdManager.h"
const ClassPropertyData SunLight::g_Properties[] = 
{
{ "ObjectID", offsetof(SunLight, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "Name", offsetof(SunLight, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, }, 
{ "UseShadowMap", offsetof(SunLight, m_UseShadowMap), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Time", offsetof(SunLight, m_Time), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "Orientation", offsetof(SunLight, m_Orientation), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "Speed", offsetof(SunLight, m_Speed), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "UseSkybox", offsetof(SunLight, m_UseSkybox), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Skybox", offsetof(SunLight, m_Skybox), 0, LXType_Texture, true, LXType_None, PropertyFlags_Hidden, 0, 0, }, 
};
void SunLight::Serialize(bool writing)
{
	g_Serializer->SerializeObject(this, writing); 
}

#pragma endregion  CLASS_SOURCE SunLight
SunLight::SunLight()
{
	m_ObjectID = g_ObjectIDManager->GetNewObjectID();
}

glm::vec3 SunLight::GetSunDirection()
{
	glm::vec2 sunDirFlat = glm::vec2(cos(m_Orientation), sin(m_Orientation));
	return cos(m_Time) * glm::vec3(0, 0, 1) + sin(m_Time) * glm::vec3(sunDirFlat.x, sunDirFlat.y, 0);
}
