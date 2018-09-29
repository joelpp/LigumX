#pragma once

#include "stdafx.h"

#pragma region  FORWARD_DECLARATIONS PostEffects
#include "property.h"

class PostEffects;


#pragma endregion  FORWARD_DECLARATIONS PostEffects
class PostEffects
{
public:
public:
public:
#pragma region  HEADER PostEffects
public:
static const int ClassID = 3178515267;
static const LXType Type = LXType_PostEffects;
static constexpr const char* ClassName = "PostEffects";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
bool GetGammaCorrectionEnabled() { return m_GammaCorrectionEnabled; }; 
void SetGammaCorrectionEnabled(bool value) { m_GammaCorrectionEnabled = value; }; 
float GetGammaExponent() { return m_GammaExponent; }; 
void SetGammaExponent(float value) { m_GammaExponent = value; }; 
bool GetToneMappingEnabled() { return m_ToneMappingEnabled; }; 
void SetToneMappingEnabled(bool value) { m_ToneMappingEnabled = value; }; 
bool GetEmissiveGlowEnabled() { return m_EmissiveGlowEnabled; }; 
void SetEmissiveGlowEnabled(bool value) { m_EmissiveGlowEnabled = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_GammaCorrectionEnabled = false;
float m_GammaExponent = 0.f;
bool m_ToneMappingEnabled = false;
bool m_EmissiveGlowEnabled = false;
public:
static const int g_PropertyCount = 6;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_PostEffectsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_GammaCorrectionEnabled,
PIDX_GammaExponent,
PIDX_ToneMappingEnabled,
PIDX_EmissiveGlowEnabled,
};
bool Serialize(bool writing);

#pragma endregion  HEADER PostEffects
PostEffects::PostEffects();

};
