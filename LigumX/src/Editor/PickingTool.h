#pragma once
#include "glm\glm.hpp"
#include "EditorTool.h"

#pragma region  FORWARD_DECLARATIONS PickingTool
#include "LXSystem.h"
class Serializer2;

class PickingTool;
class Entity;
class Sector;


#pragma endregion  FORWARD_DECLARATIONS PickingTool

#pragma region  HEADER PickingTool
class PickingTool : public EditorTool
{
public:
static const int ClassID = -315061750;
static const LXType Type = LXType_PickingTool;
static constexpr const char* ClassName = "PickingTool";
virtual LXType GetLXType() { return LXType_PickingTool; }
virtual const char* GetLXClassName() { return ClassName; }
typedef EditorTool super;

bool GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { SetPickedEntity_Callback(value); }; 
void SetPickedEntity_Callback(Entity* value);
glm::vec2& GetAimingWindowPosition() { return m_AimingWindowPosition; }; 
void SetAimingWindowPosition(const glm::vec2& value) { m_AimingWindowPosition = value; }; 
glm::vec3& GetAimingWorldPosition() { return m_AimingWorldPosition; }; 
void SetAimingWorldPosition(const glm::vec3& value) { m_AimingWorldPosition = value; }; 
float GetAimingID() { return m_AimingID; }; 
void SetAimingID(float value) { m_AimingID = value; }; 
glm::vec3& GetPickedWorldPosition() { return m_PickedWorldPosition; }; 
void SetPickedWorldPosition(const glm::vec3& value) { m_PickedWorldPosition = value; }; 
glm::vec4& GetPickingData() { return m_PickingData; }; 
void SetPickingData(const glm::vec4& value) { m_PickingData = value; }; 
float GetPickedDepth() { return m_PickedDepth; }; 
void SetPickedDepth(float value) { m_PickedDepth = value; }; 
float GetPickedID() { return m_PickedID; }; 
void SetPickedID(float value) { m_PickedID = value; }; 
bool GetPickDebugModels() { return m_PickDebugModels; }; 
void SetPickDebugModels(bool value) { m_PickDebugModels = value; }; 
float GetPickedHeight() { return m_PickedHeight; }; 
void SetPickedHeight(float value) { m_PickedHeight = value; }; 
Sector*& GetPickedSector() { return m_PickedSector; }; 
void SetPickedSector(Sector* value) { m_PickedSector = value; }; 
bool GetCOMMAND_CloneSelectAndAddToWorld() { return m_COMMAND_CloneSelectAndAddToWorld; }; 
void SetCOMMAND_CloneSelectAndAddToWorld(bool value) { SetCOMMAND_CloneSelectAndAddToWorld_Callback(value); }; 
void SetCOMMAND_CloneSelectAndAddToWorld_Callback(const bool& value);
private:
bool m_Enabled = true;
Entity* m_PickedEntity = nullptr;
glm::vec2 m_AimingWindowPosition = glm::vec2(0, 0);
glm::vec3 m_AimingWorldPosition = glm::vec3(0, 0, 0);
float m_AimingID = 0.f;
glm::vec3 m_PickedWorldPosition = glm::vec3(0, 0, 0);
glm::vec4 m_PickingData;
float m_PickedDepth = 0.f;
float m_PickedID = 0.f;
bool m_PickDebugModels = false;
float m_PickedHeight = 0.f;
Sector* m_PickedSector = nullptr;
bool m_COMMAND_CloneSelectAndAddToWorld = false;
public:
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_PickingToolPIDX
{
PIDX_Enabled,
PIDX_PickedEntity,
PIDX_AimingWindowPosition,
PIDX_AimingWorldPosition,
PIDX_AimingID,
PIDX_PickedWorldPosition,
PIDX_PickingData,
PIDX_PickedDepth,
PIDX_PickedID,
PIDX_PickDebugModels,
PIDX_PickedHeight,
PIDX_PickedSector,
PIDX_COMMAND_CloneSelectAndAddToWorld,
};
virtual void Serialize(Serializer2& serializer);
virtual bool Serialize(bool writing);
virtual bool ShowPropertyGrid();
virtual void Clone(LXObject* otherObj);
virtual const char* GetTypeName();

#pragma endregion  HEADER PickingTool

bool Process(bool isActiveTool, bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance) override;

void UpdatePickingData();

bool IsPickingEntity();

bool CheckEntity(Entity* entity);

};