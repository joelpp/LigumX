#pragma once

#include "stdafx.h"

#pragma region  FORWARD_DECLARATIONS PostEffects
#include "property.h"

#pragma endregion  FORWARD_DECLARATIONS PostEffects
class PostEffects
{
public:
public:
public:
#pragma region  HEADER PostEffects
public:
static const int ClassID = 3178515267;
static constexpr const char* ClassName = "PostEffects";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const bool& GetGammaCorrectionEnabled() { return m_GammaCorrectionEnabled; }; 
void SetGammaCorrectionEnabled(bool value) { m_GammaCorrectionEnabled = value; }; 
const float& GetGammaExponent() { return m_GammaExponent; }; 
void SetGammaExponent(float value) { m_GammaExponent = value; }; 
const bool& GetToneMappingEnabled() { return m_ToneMappingEnabled; }; 
void SetToneMappingEnabled(bool value) { m_ToneMappingEnabled = value; }; 
private:
int m_ObjectID;
bool m_GammaCorrectionEnabled;
float m_GammaExponent;
bool m_ToneMappingEnabled;
public:
static const int g_PropertyCount = 4;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_PostEffectsPIDX
{
PIDX_ObjectID,
PIDX_GammaCorrectionEnabled,
PIDX_GammaExponent,
PIDX_ToneMappingEnabled,
};
void Serialize(bool writing);

#pragma endregion  HEADER PostEffects
PostEffects::PostEffects();

};
