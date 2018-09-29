#pragma once

#pragma region  FORWARD_DECLARATIONS DebugRenderer
#include "property.h"

class DebugRenderer;

extern DebugRenderer* g_DebugRenderer;

#pragma endregion  FORWARD_DECLARATIONS DebugRenderer

class DebugRenderer
{
#pragma region  HEADER DebugRenderer
public:
static const int ClassID = 1763775919;
static const LXType Type = LXType_DebugRenderer;
static constexpr const char* ClassName = "DebugRenderer";

int GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = false;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DebugRendererPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
};
bool Serialize(bool writing);

#pragma endregion  HEADER DebugRenderer

};
