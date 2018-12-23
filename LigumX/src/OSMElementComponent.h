#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS OSMElementComponent
#include "LXSystem.h"

class OSMElementComponent;
class Way;


#pragma endregion  FORWARD_DECLARATIONS OSMElementComponent

#pragma region  HEADER OSMElementComponent
class OSMElementComponent : public Component
{
public:
static const int ClassID = 3716861205;
static const LXType Type = LXType_OSMElementComponent;
static constexpr const char* ClassName = "OSMElementComponent";
typedef Component super;

Way*& GetWay() { return m_Way; }; 
void SetWay(Way* value) { m_Way = value; }; 
private:
Way* m_Way = nullptr;
public:
static const int g_PropertyCount = 1;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementComponentPIDX
{
PIDX_Way,
};
bool Serialize(bool writing);
bool ShowPropertyGrid();

#pragma endregion  HEADER OSMElementComponent

void Update();

};
