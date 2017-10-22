#pragma once

#pragma region  FORWARD_DECLARATIONS SunLight
#include "property.h"
class Texture;

#pragma endregion  FORWARD_DECLARATIONS SunLight
class SunLight
{
public:
public:
public:
#pragma region  HEADER SunLight
public:
static const int ClassID = 2842600075;
static constexpr const char* ClassName = "SunLight";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
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
int m_ObjectID;
bool m_UseShadowMap;
float m_Time;
float m_Orientation;
float m_Speed;
bool m_UseSkybox;
Texture* m_Skybox;
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

SunLight::SunLight();
enum g_SunLightPIDX
{
PIDX_ObjectID,
PIDX_UseShadowMap,
PIDX_Time,
PIDX_Orientation,
PIDX_Speed,
PIDX_UseSkybox,
PIDX_Skybox,
};
void Serialize(bool writing);

#pragma endregion  HEADER SunLight

glm::vec3 GetSunDirection();

};
