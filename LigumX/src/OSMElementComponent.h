#pragma once

#include "Component.h"

#pragma region  FORWARD_DECLARATIONS OSMElementComponent
#include "LXSystem.h"
class Serializer2;

class OSMElementComponent;
class Way;
class Model;


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
Model*& GetDebugWayModel() { return m_DebugWayModel; }; 
void SetDebugWayModel(Model* value) { m_DebugWayModel = value; }; 
bool GetCOMMAND_CreateDebugMesh() { return m_COMMAND_CreateDebugMesh; }; 
void SetCOMMAND_CreateDebugMesh(bool value) { SetCOMMAND_CreateDebugMesh_Callback(value); }; 
void SetCOMMAND_CreateDebugMesh_Callback(const bool& value);
private:
Way* m_Way = nullptr;
Model* m_DebugWayModel = nullptr;
bool m_COMMAND_CreateDebugMesh = false;
public:
static const int g_PropertyCount = 3;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_OSMElementComponentPIDX
{
PIDX_Way,
PIDX_DebugWayModel,
PIDX_COMMAND_CreateDebugMesh,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER OSMElementComponent

void Update();
virtual void DebugDisplay();

};
