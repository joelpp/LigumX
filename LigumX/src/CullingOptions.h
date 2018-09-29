#pragma once

#pragma region  FORWARD_DECLARATIONS CullingOptions
#include "property.h"

class CullingOptions;


#pragma endregion  FORWARD_DECLARATIONS CullingOptions

class CullingOptions
{
#pragma region  HEADER CullingOptions
public:
static const int ClassID = 188449055;
static const LXType Type = LXType_CullingOptions;
static constexpr const char* ClassName = "CullingOptions";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
bool GetCullEntities() { return m_CullEntities; }; 
void SetCullEntities(bool value) { m_CullEntities = value; }; 
bool GetCullSectors() { return m_CullSectors; }; 
void SetCullSectors(bool value) { m_CullSectors = value; }; 
bool GetUseDotProduct() { return m_UseDotProduct; }; 
void SetUseDotProduct(bool value) { m_UseDotProduct = value; }; 
bool GetUseAABBClipPos() { return m_UseAABBClipPos; }; 
void SetUseAABBClipPos(bool value) { m_UseAABBClipPos = value; }; 
bool GetDebugAABBClippPos() { return m_DebugAABBClippPos; }; 
void SetDebugAABBClippPos(bool value) { m_DebugAABBClippPos = value; }; 
bool GetDebugDotProduct() { return m_DebugDotProduct; }; 
void SetDebugDotProduct(bool value) { m_DebugDotProduct = value; }; 
float GetDebugDotProductMaxDistance() { return m_DebugDotProductMaxDistance; }; 
void SetDebugDotProductMaxDistance(float value) { m_DebugDotProductMaxDistance = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_CullEntities = false;
bool m_CullSectors = false;
bool m_UseDotProduct = false;
bool m_UseAABBClipPos = true;
bool m_DebugAABBClippPos = false;
bool m_DebugDotProduct = false;
float m_DebugDotProductMaxDistance = 30.f;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_CullingOptionsPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_CullEntities,
PIDX_CullSectors,
PIDX_UseDotProduct,
PIDX_UseAABBClipPos,
PIDX_DebugAABBClippPos,
PIDX_DebugDotProduct,
PIDX_DebugDotProductMaxDistance,
};
bool Serialize(bool writing);

#pragma endregion  HEADER CullingOptions

};
