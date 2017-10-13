#pragma once

#include "stdafx.h"

#pragma region  FORWARD_DECLARATIONS PostEffects
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS PostEffects
class PostEffects
{
#pragma region  HEADER PostEffects
static const int ClassID = 3178515267;
public:
const bool& GetGammaCorrectionEnabled() { return m_GammaCorrectionEnabled; }; 
void SetGammaCorrectionEnabled(bool value) { m_GammaCorrectionEnabled = value; }; 
const float& GetGammaExponent() { return m_GammaExponent; }; 
void SetGammaExponent(float value) { m_GammaExponent = value; }; 
const bool& GetToneMappingEnabled() { return m_ToneMappingEnabled; }; 
void SetToneMappingEnabled(bool value) { m_ToneMappingEnabled = value; }; 
private:
bool m_GammaCorrectionEnabled;
float m_GammaExponent;
bool m_ToneMappingEnabled;
public:
static const int g_PostEffectsPropertyCount = 3;
static const ClassPropertyData g_Properties[g_PostEffectsPropertyCount];


#pragma endregion  HEADER PostEffects

};
