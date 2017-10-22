#include "stdafx.h"
#include "glm\glm.hpp"

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include <cstddef>

#pragma region  CLASS_SOURCE SunLight
#include "SunLight.h"
#include "serializer.h"
#include <cstddef>
const ClassPropertyData SunLight::g_Properties[] = 
{
{ "ObjectID", offsetof(SunLight, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, }, 
{ "UseShadowMap", offsetof(SunLight, m_UseShadowMap), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Time", offsetof(SunLight, m_Time), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "Orientation", offsetof(SunLight, m_Orientation), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "Speed", offsetof(SunLight, m_Speed), 0, LXType_float, false, LXType_None, 0, 0, 0, }, 
{ "UseSkybox", offsetof(SunLight, m_UseSkybox), 0, LXType_bool, false, LXType_None, 0, 0, 0, }, 
{ "Skybox", offsetof(SunLight, m_Skybox), 0, LXType_Texture, true, LXType_None, PropertyFlags_Hidden, 0, 0, }, 
};
void SunLight::Serialize(bool writing)
{
	std::string basePath = "C:\\Users\\Joel\\Documents\\LigumX\\LigumX\\data\\objects\\";
	std::string fileName = "SunLight_" + std::to_string(m_ObjectID) + ".LXobj";

	int fileMask = writing ? std::ios::out : std::ios::in;
	std::fstream objectStream(basePath + fileName, fileMask);

	if (objectStream.is_open())
	{
		Serializer::SerializeObject(this, objectStream, writing);
	}
}

#pragma endregion  CLASS_SOURCE SunLight
SunLight::SunLight()
{
	m_ObjectID = rand();
}

glm::vec3 SunLight::GetSunDirection()
{
	glm::vec2 sunDirFlat = glm::vec2(cos(m_Orientation), sin(m_Orientation));
	return cos(m_Time) * glm::vec3(0, 0, 1) + sin(m_Time) * glm::vec3(sunDirFlat.x, sunDirFlat.y, 0);
}
