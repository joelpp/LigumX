#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS OSMElementComponent
#include "property.h"

class OSMElementComponent;
class Way;


#pragma endregion  FORWARD_DECLARATIONS OSMElementComponent

class OSMElementComponent : public Component
{
#pragma region  HEADER OSMElementComponent
public:
static const int ClassID = 3716861205;
static const LXType Type = LXType_OSMElementComponent;
static constexpr const char* ClassName = "OSMElementComponent";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
Way*& GetWay() { return m_Way; }; 
void SetWay(Way* value) { m_Way = value; }; 
private:
int m_ObjectID;
std::string m_Name;
Way* m_Way = nullptr;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementComponentPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Way,
};
bool Serialize(bool writing);

#pragma endregion  HEADER OSMElementComponent

void Update();

};
