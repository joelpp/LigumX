#pragma once

#include "EditorOptions.h"
#include "glm/glm.hpp"


#pragma region  FORWARD_DECLARATIONS Editor
#include "property.h"

class Editor;
class EditorOptions;
class Entity;

extern Editor* g_Editor;

#pragma endregion  FORWARD_DECLARATIONS Editor

class Editor
{
#pragma region  HEADER Editor
public:
static const int ClassID = 1231601560;
static const LXType Type = LXType_Editor;
static constexpr const char* ClassName = "Editor";

const int& GetObjectID() { return m_ObjectID; }; 
void SetObjectID(int value) { m_ObjectID = value; }; 
const std::string& GetName() { return m_Name; }; 
void SetName(std::string value) { m_Name = value; }; 
EditorOptions*& GetOptions() { return m_Options; }; 
void SetOptions(EditorOptions* value) { m_Options = value; }; 
const glm::vec2& GetMouseClickPosition() { return m_MouseClickPosition; }; 
void SetMouseClickPosition(glm::vec2 value) { m_MouseClickPosition = value; }; 
const glm::vec2& GetLastMouseClickPosition() { return m_LastMouseClickPosition; }; 
void SetLastMouseClickPosition(glm::vec2 value) { m_LastMouseClickPosition = value; }; 
const glm::vec2& GetLastMousePosition() { return m_LastMousePosition; }; 
void SetLastMousePosition(glm::vec2 value) { m_LastMousePosition = value; }; 
const glm::vec2& GetMousePosition() { return m_MousePosition; }; 
void SetMousePosition(glm::vec2 value) { m_MousePosition = value; }; 
const bool& GetMouseButton1Down() { return m_MouseButton1Down; }; 
void SetMouseButton1Down(bool value) { m_MouseButton1Down = value; }; 
const glm::vec3& GetXYZMask() { return m_XYZMask; }; 
void SetXYZMask(glm::vec3 value) { m_XYZMask = value; }; 
void AddToXYZMask(glm::vec3 value) { m_XYZMask += value; };
Entity*& GetPickedEntity() { return m_PickedEntity; }; 
void SetPickedEntity(Entity* value) { m_PickedEntity = value; }; 
const bool& GetManipulatorDragging() { return m_ManipulatorDragging; }; 
void SetManipulatorDragging(bool value) { m_ManipulatorDragging = value; }; 
const int& GetPickingBufferSize() { return m_PickingBufferSize; }; 
void SetPickingBufferSize(int value) { m_PickingBufferSize = value; }; 
private:
int m_ObjectID;
std::string m_Name;
EditorOptions* m_Options;
glm::vec2 m_MouseClickPosition;
glm::vec2 m_LastMouseClickPosition;
glm::vec2 m_LastMousePosition;
glm::vec2 m_MousePosition;
bool m_MouseButton1Down;
glm::vec3 m_XYZMask;
Entity* m_PickedEntity;
bool m_ManipulatorDragging;
int m_PickingBufferSize;
public:
static const int g_PropertyCount = 12;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_EditorPIDX
{
PIDX_ObjectID,
PIDX_Name,
PIDX_Options,
PIDX_MouseClickPosition,
PIDX_LastMouseClickPosition,
PIDX_LastMousePosition,
PIDX_MousePosition,
PIDX_MouseButton1Down,
PIDX_XYZMask,
PIDX_PickedEntity,
PIDX_ManipulatorDragging,
PIDX_PickingBufferSize,
};
bool Serialize(bool writing);

#pragma endregion  HEADER Editor

Editor();
Editor(int ObjectID);

void RenderPicking();

};
