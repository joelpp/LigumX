#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS OSMElementComponent
#include "LXSystem.h"
class Serializer2;

class OSMElementComponent;
class Way;


#pragma endregion  FORWARD_DECLARATIONS OSMElementComponent

#pragma region  HEADER OSMElementComponent
class OSMElementComponent : public Component
{
public:
static const int ClassID = -777561099;
static const LXType Type = LXType_OSMElementComponent;
static constexpr const char* ClassName = "OSMElementComponent";
virtual LXType GetLXType() { return LXType_OSMElementComponent; }
virtual const char* GetLXClassName() { return ClassName; }
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
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(OSMElementComponent& other);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMElementComponent

void Update();

};
