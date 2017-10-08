#pragma once

#pragma region  FORWARD_DECLARATIONS SunLight
#include "property.h"
class Texture;

#pragma endregion  FORWARD_DECLARATIONS SunLight
class SunLight
{
#pragma region  HEADER SunLight
static const int ClassID = 2842600075;
public:
const bool& GetUseShadowMap() { return m_UseShadowMap; }; 
void SetUseShadowMap(bool value) { m_UseShadowMap = value; }; 
const float& GetTime() { return m_Time; }; 
void SetTime(float value) { m_Time = value; }; 
const float& GetOrientation() { return m_Orientation; }; 
void SetOrientation(float value) { m_Orientation = value; }; 
const float& GetSpeed() { return m_Speed; }; 
void SetSpeed(float value) { m_Speed = value; }; 
const bool& GetUseSkybox() { return m_UseSkybox; }; 
void SetUseSkybox(bool value) { m_UseSkybox = value; }; 
Texture* GetSkybox() { return m_Skybox; }; 
void SetSkybox(Texture* value) { m_Skybox = value; }; 
private:
bool m_UseShadowMap;
float m_Time;
float m_Orientation;
float m_Speed;
bool m_UseSkybox;
Texture* m_Skybox;
public:
static const int g_SunLightPropertyCount = 6;
static const ClassPropertyData g_Properties[g_SunLightPropertyCount];

SunLight::SunLight();

#pragma endregion  HEADER SunLight

glm::vec3 GetSunDirection();

};
