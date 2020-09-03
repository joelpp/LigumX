#pragma once

#pragma region  FORWARD_DECLARATIONS SunLight
#include "LXSystem.h"
class Serializer2;

class SunLight;
class Texture;
class ShadowParameters;


#pragma endregion  FORWARD_DECLARATIONS SunLight

#pragma region  HEADER SunLight
class SunLight : public LXObject
{
public:
static const int ClassID = 1479238507;
static const LXType Type = LXType_SunLight;
static constexpr const char* ClassName = "SunLight";
virtual LXType GetLXType() { return LXType_SunLight; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetUseShadowMap() { return m_UseShadowMap; }; 
void SetUseShadowMap(bool value) { m_UseShadowMap = value; }; 
float GetTime() { return m_Time; }; 
void SetTime(float value) { m_Time = value; }; 
float GetOrientation() { return m_Orientation; }; 
void SetOrientation(float value) { m_Orientation = value; }; 
float GetSpeed() { return m_Speed; }; 
void SetSpeed(float value) { m_Speed = value; }; 
bool GetUseSkybox() { return m_UseSkybox; }; 
void SetUseSkybox(bool value) { SetUseSkybox_Callback(value); }; 
void SetUseSkybox_Callback(const bool& value);
Texture*& GetSkybox() { return m_Skybox; }; 
void SetSkybox(Texture* value) { m_Skybox = value; }; 
ShadowParameters*& GetShadowParameters() { return m_ShadowParameters; }; 
void SetShadowParameters(ShadowParameters* value) { m_ShadowParameters = value; }; 
private:
bool m_UseShadowMap = false;
float m_Time = 0.f;
float m_Orientation = 0.f;
float m_Speed = 0.f;
bool m_UseSkybox = false;
Texture* m_Skybox = nullptr;
ShadowParameters* m_ShadowParameters = nullptr;
public:
static const int g_PropertyCount = 7;
static const ClassPropertyData g_Properties[g_PropertyCount];

SunLight::SunLight();
enum g_SunLightPIDX
{
PIDX_UseShadowMap,
PIDX_Time,
PIDX_Orientation,
PIDX_Speed,
PIDX_UseSkybox,
PIDX_Skybox,
PIDX_ShadowParameters,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER SunLight

glm::vec3 GetSunDirection();



};
