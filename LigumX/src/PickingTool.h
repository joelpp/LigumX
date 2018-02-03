#pragma once
#include "glm\glm.hpp"

#pragma region  FORWARD_DECLARATIONS PickingTool
#include "property.h"

class PickingTool;
class Entity;


#pragma endregion  FORWARD_DECLARATIONS PickingTool

class PickingTool
{
#pragma region  HEADER PickingTool
public:
static const int ClassID = 395171594;
static const LXType Type = LXType_PickingTool;
static constexpr const char* ClassName = "PickingTool";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
const bool& GetEnabled() { return m_Enabled; }; 
void SetEnabled(bool value) { m_Enabled = value; }; 
Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { m_PickedEntity = value; }; 
const glm::vec3& GetPickedWorldPosition() { return m_PickedWorldPosition; }; 
void SetPickedWorldPosition(glm::vec3 value) { m_PickedWorldPosition = value; }; 
const glm::vec4& GetPickingData() { return m_PickingData; }; 
void SetPickingData(glm::vec4 value) { m_PickingData = value; }; 
const float& GetPickedDepth() { return m_PickedDepth; }; 
void SetPickedDepth(float value) { m_PickedDepth = value; }; 
const float& GetPickedID() { return m_PickedID; }; 
void SetPickedID(float value) { m_PickedID = value; }; 
const bool& GetPickDebugModels() { return m_PickDebugModels; }; 
void SetPickDebugModels(bool value) { m_PickDebugModels = value; }; 
private:
int m_ObjectID;
std::string m_Name;
bool m_Enabled = true;
Entity* m_PickedEntity;
glm::vec3 m_PickedWorldPosition = glm::vec3(0, 0, 0);
glm::vec4 m_PickingData;
float m_PickedDepth = 0.f;
float m_PickedID = 0.f;
bool m_PickDebugModels = false;
public:
static const int g_PropertyCount = 9;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_PickingToolPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Enabled,
PIDX_PickedEntity,
PIDX_PickedWorldPosition,
PIDX_PickingData,
PIDX_PickedDepth,
PIDX_PickedID,
PIDX_PickDebugModels,
};
bool Serialize(bool writing);

#pragma endregion  HEADER PickingTool

bool Process(bool mouseButton1Down, const glm::vec2& mousePosition, const glm::vec2& dragDistance);

void UpdatePickingData();

};
