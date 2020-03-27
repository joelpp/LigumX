#pragma once

#include "stdafx.h"

#pragma region  FORWARD_DECLARATIONS PostEffects
#include "LXSystem.h"
class Serializer2;

class PostEffects;


#pragma endregion  FORWARD_DECLARATIONS PostEffects

#pragma region  HEADER PostEffects
class PostEffects : public LXObject
{
public:
static const int ClassID = -1116452029;
static const LXType Type = LXType_PostEffects;
static constexpr const char* ClassName = "PostEffects";
typedef LXObject super;

bool GetGammaCorrectionEnabled() { return m_GammaCorrectionEnabled; }; 
void SetGammaCorrectionEnabled(bool value) { m_GammaCorrectionEnabled = value; }; 
float GetGammaExponent() { return m_GammaExponent; }; 
void SetGammaExponent(float value) { m_GammaExponent = value; }; 
bool GetToneMappingEnabled() { return m_ToneMappingEnabled; }; 
void SetToneMappingEnabled(bool value) { m_ToneMappingEnabled = value; }; 
bool GetEmissiveGlowEnabled() { return m_EmissiveGlowEnabled; }; 
void SetEmissiveGlowEnabled(bool value) { m_EmissiveGlowEnabled = value; }; 
private:
bool m_GammaCorrectionEnabled = false;
float m_GammaExponent = 0.f;
bool m_ToneMappingEnabled = false;
bool m_EmissiveGlowEnabled = false;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_PostEffectsPIDX
{
PIDX_GammaCorrectionEnabled,
PIDX_GammaExponent,
PIDX_ToneMappingEnabled,
PIDX_EmissiveGlowEnabled,
};
void Serialize(Serializer2& serializer);
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual const char* GetTypeName();

#pragma endregion  HEADER PostEffects
PostEffects::PostEffects();

};
