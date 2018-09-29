#pragma once

#pragma region  FORWARD_DECLARATIONS SunLight
#include "property.h"

class SunLight;
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
static const LXType Type = LXType_SunLight;
static constexpr const char* ClassName = "SunLight";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
bool GetUseShadowMap() { return m_UseShadowMap; }; 
void SetUseShadowMap(bool value) { m_UseShadowMap = value; }; 
float GetTime() { return m_Time; }; 
void SetTime(float value) { m_Time = value; }; 
float GetOrientation() { return m_Orientation; }; 
void SetOrientation(float value) { m_Orientation = value; }; 
float GetSpeed() { return m_Speed; }; 
void SetSpeed(float value) { m_Speed = value; }; 
bool GetUseSkybox() { return m_UseSkybox; }; 
void SetUseSkybox(bool value) { m_UseSkybox = value; }; 
Texture*& GetSkybox() { return m_Skybox; }; 
void SetSkybox(Texture* value) { m_Skybox = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_UseShadowMap = false;
float m_Time = 0.f;
float m_Orientation = 0.f;
float m_Speed = 0.f;
bool m_UseSkybox = false;
Texture* m_Skybox = nullptr;
public:
static const int g_PropertyCount = 8;
static const ClassPropertyData g_Properties[g_PropertyCount];

SunLight::SunLight();
enum g_SunLightPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_UseShadowMap,
PIDX_Time,
PIDX_Orientation,
PIDX_Speed,
PIDX_UseSkybox,
PIDX_Skybox,
};
bool Serialize(bool writing);

#pragma endregion  HEADER SunLight

glm::vec3 GetSunDirection();

};
