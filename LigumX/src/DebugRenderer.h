#pragma once

#pragma region  FORWARD_DECLARATIONS DebugRenderer
#include "LXSystem.h"

class DebugRenderer;

extern DebugRenderer* g_DebugRenderer;

#pragma endregion  FORWARD_DECLARATIONS DebugRenderer

#pragma region  HEADER DebugRenderer
class DebugRenderer : public LXObject
{
public:
static const int ClassID = 1763775919;
static const LXType Type = LXType_DebugRenderer;
static constexpr const char* ClassName = "DebugRenderer";
typedef LXObject super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
private:
bool m_Enabled = false;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_DebugRendererPIDX
{
PIDX_Enabled,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();

#pragma endregion  HEADER DebugRenderer

};
