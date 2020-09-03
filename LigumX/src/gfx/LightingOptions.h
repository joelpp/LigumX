#pragma once

#pragma region  FORWARD_DECLARATIONS LightingOptions
#include "LXSystem.h"
class Serializer2;

class LightingOptions;


#pragma endregion  FORWARD_DECLARATIONS LightingOptions
#pragma region  HEADER LightingOptions
class LightingOptions : public LXObject
{
public:
static const int ClassID = -1477375727;
static const LXType Type = LXType_LightingOptions;
static constexpr const char* ClassName = "LightingOptions";
virtual LXType GetLXType() { return LXType_LightingOptions; }
virtual const char* GetLXClassName() { return ClassName; }
typedef LXObject super;

bool GetEnableDynamicLights() { return m_EnableDynamicLights; }; 
void SetEnableDynamicLights(bool value) { m_EnableDynamicLights = value; }; 
bool GetEnableSunlight() { return m_EnableSunlight; }; 
void SetEnableSunlight(bool value) { m_EnableSunlight = value; }; 
bool GetEnableAmbientLighting() { return m_EnableAmbientLighting; }; 
void SetEnableAmbientLighting(bool value) { m_EnableAmbientLighting = value; }; 
float GetAmbientLightingGlobalFactor() { return m_AmbientLightingGlobalFactor; }; 
void SetAmbientLightingGlobalFactor(float value) { m_AmbientLightingGlobalFactor = value; }; 
bool GetEnableDiffuseComponent() { return m_EnableDiffuseComponent; }; 
void SetEnableDiffuseComponent(bool value) { m_EnableDiffuseComponent = value; }; 
bool GetEnableSpecularComponent() { return m_EnableSpecularComponent; }; 
void SetEnableSpecularComponent(bool value) { m_EnableSpecularComponent = value; }; 
bool GetEnableReflection() { return m_EnableReflection; }; 
void SetEnableReflection(bool value) { m_EnableReflection = value; }; 
bool GetEnableSunShadow() { return m_EnableSunShadow; }; 
void SetEnableSunShadow(bool value) { m_EnableSunShadow = value; }; 
bool GetEnableDynamicShadows() { return m_EnableDynamicShadows; }; 
void SetEnableDynamicShadows(bool value) { m_EnableDynamicShadows = value; }; 
bool GetEnableReplacementAlbedo() { return m_EnableReplacementAlbedo; }; 
void SetEnableReplacementAlbedo(bool value) { m_EnableReplacementAlbedo = value; }; 
glm::vec3& GetReplacementAlbedo() { return m_ReplacementAlbedo; }; 
void SetReplacementAlbedo(const glm::vec3& value) { m_ReplacementAlbedo = value; }; 
bool GetEnableReplacementMetallic() { return m_EnableReplacementMetallic; }; 
void SetEnableReplacementMetallic(bool value) { m_EnableReplacementMetallic = value; }; 
float GetReplacementMetallic() { return m_ReplacementMetallic; }; 
void SetReplacementMetallic(float value) { m_ReplacementMetallic = value; }; 
bool GetEnableReplacementRoughness() { return m_EnableReplacementRoughness; }; 
void SetEnableReplacementRoughness(bool value) { m_EnableReplacementRoughness = value; }; 
float GetReplacementRoughness() { return m_ReplacementRoughness; }; 
void SetReplacementRoughness(float value) { m_ReplacementRoughness = value; }; 
private:
bool m_EnableDynamicLights = true;
bool m_EnableSunlight = true;
bool m_EnableAmbientLighting = true;
float m_AmbientLightingGlobalFactor = 1.f;
bool m_EnableDiffuseComponent = true;
bool m_EnableSpecularComponent = true;
bool m_EnableReflection = true;
bool m_EnableSunShadow = true;
bool m_EnableDynamicShadows = true;
bool m_EnableReplacementAlbedo = false;
glm::vec3 m_ReplacementAlbedo = glm::vec3(0, 0, 0);
bool m_EnableReplacementMetallic = false;
float m_ReplacementMetallic = 0.f;
bool m_EnableReplacementRoughness = false;
float m_ReplacementRoughness = 0.f;
public:
static const int g_PropertyCount = 15;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_LightingOptionsPIDX
{
PIDX_EnableDynamicLights,
PIDX_EnableSunlight,
PIDX_EnableAmbientLighting,
PIDX_AmbientLightingGlobalFactor,
PIDX_EnableDiffuseComponent,
PIDX_EnableSpecularComponent,
PIDX_EnableReflection,
PIDX_EnableSunShadow,
PIDX_EnableDynamicShadows,
PIDX_EnableReplacementAlbedo,
PIDX_ReplacementAlbedo,
PIDX_EnableReplacementMetallic,
PIDX_ReplacementMetallic,
PIDX_EnableReplacementRoughness,
PIDX_ReplacementRoughness,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER LightingOptions



};
