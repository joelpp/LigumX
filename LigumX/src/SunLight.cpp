#include "stdafx.h"
#include "glm\glm.hpp"

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include <cstddef>
const ClassPropertyData SunLight::g_Properties[] = 
{
{ "UseShadowMap", offsetof(SunLight, m_UseShadowMap), 0, LXType_bool, false,  }, 
{ "Time", offsetof(SunLight, m_Time), 0, LXType_float, false,  }, 
{ "Orientation", offsetof(SunLight, m_Orientation), 0, LXType_float, false,  }, 
{ "Speed", offsetof(SunLight, m_Speed), 0, LXType_float, false,  }, 
};
SunLight::SunLight() { }

#pragma endregion  CLASS_SOURCE SunLight

glm::vec3 SunLight::GetSunDirection()
{
	glm::vec2 sunDirFlat = glm::vec2(cos(m_Orientation), sin(m_Orientation));
	return cos(m_Time) * glm::vec3(0, 0, 1) + sin(m_Time) * glm::vec3(sunDirFlat.x, sunDirFlat.y, 0);
}
