#pragma once

#include "glm\glm.hpp"
#include "Renderer.h"

#pragma region  FORWARD_DECLARATIONS InputHandler
#include "LXSystem.h"

class InputHandler;

extern InputHandler* g_InputHandler;

#pragma endregion  FORWARD_DECLARATIONS InputHandler

#pragma region  HEADER InputHandler
class InputHandler : public LXObject
{
public:
static const int ClassID = 2115103629;
static const LXType Type = LXType_InputHandler;
static constexpr const char* ClassName = "InputHandler";
typedef LXObject super;

bool GetMouse1Pressed() { return m_Mouse1Pressed; }; 
void SetMouse1Pressed(bool value) { SetMouse1Pressed_Callback(value); }; 
void SetMouse1Pressed_Callback(const bool& value);
bool GetMouse2Pressed() { return m_Mouse2Pressed; }; 
void SetMouse2Pressed(bool value) { m_Mouse2Pressed = value; }; 
bool GetCtrlHeld() { return m_CtrlHeld; }; 
void SetCtrlHeld(bool value) { m_CtrlHeld = value; }; 
bool GetDragging() { return m_Dragging; }; 
void SetDragging(bool value) { m_Dragging = value; }; 
glm::vec2& GetDragDistance() { return m_DragDistance; }; 
void SetDragDistance(const glm::vec2& value) { m_DragDistance = value; }; 
glm::vec2& GetLastDragDistance() { return m_LastDragDistance; }; 
void SetLastDragDistance(const glm::vec2& value) { m_LastDragDistance = value; }; 
glm::vec2& GetDragAccumulator() { return m_DragAccumulator; }; 
void SetDragAccumulator(const glm::vec2& value) { m_DragAccumulator = value; }; 
void AddTo_DragAccumulator(glm::vec2 value) { m_DragAccumulator += value; };
glm::vec2& GetMouseClickPosition() { return m_MouseClickPosition; }; 
void SetMouseClickPosition(const glm::vec2& value) { SetMouseClickPosition_Callback(value); }; 
void SetMouseClickPosition_Callback(const glm::vec2& value);
glm::vec2& GetLastMouseClickPosition() { return m_LastMouseClickPosition; }; 
void SetLastMouseClickPosition(const glm::vec2& value) { m_LastMouseClickPosition = value; }; 
glm::vec2& GetMouseReleasePosition() { return m_MouseReleasePosition; }; 
void SetMouseReleasePosition(const glm::vec2& value) { m_MouseReleasePosition = value; }; 
glm::vec2& GetMousePosition() { return m_MousePosition; }; 
void SetMousePosition(const glm::vec2& value) { SetMousePosition_Callback(value); }; 
void SetMousePosition_Callback(const glm::vec2& value);
glm::vec2& GetLastMousePosition() { return m_LastMousePosition; }; 
void SetLastMousePosition(const glm::vec2& value) { m_LastMousePosition = value; }; 
glm::vec2& GetMouseScroll() { return m_MouseScroll; }; 
void SetMouseScroll(const glm::vec2& value) { m_MouseScroll = value; }; 
void AddTo_MouseScroll(glm::vec2 value) { m_MouseScroll += value; };
private:
bool m_Mouse1Pressed = false;
bool m_Mouse2Pressed = false;
bool m_CtrlHeld = false;
bool m_Dragging = false;
glm::vec2 m_DragDistance = glm::vec2(0, 0);
glm::vec2 m_LastDragDistance = glm::vec2(0, 0);
glm::vec2 m_DragAccumulator = glm::vec2(0, 0);
glm::vec2 m_MouseClickPosition = glm::vec2(0, 0);
glm::vec2 m_LastMouseClickPosition = glm::vec2(0, 0);
glm::vec2 m_MouseReleasePosition = glm::vec2(0, 0);
glm::vec2 m_MousePosition = glm::vec2(0, 0);
glm::vec2 m_LastMousePosition = glm::vec2(0, 0);
glm::vec2 m_MouseScroll = glm::vec2(0, 0);
public:
static const int g_PropertyCount = 13;
static const ClassPropertyData g_Properties[g_PropertyCount];

enum g_InputHandlerPIDX
{
PIDX_Mouse1Pressed,
PIDX_Mouse2Pressed,
PIDX_CtrlHeld,
PIDX_Dragging,
PIDX_DragDistance,
PIDX_LastDragDistance,
PIDX_DragAccumulator,
PIDX_MouseClickPosition,
PIDX_LastMouseClickPosition,
PIDX_MouseReleasePosition,
PIDX_MousePosition,
PIDX_LastMousePosition,
PIDX_MouseScroll,
};
bool Serialize(bool writing);
virtual bool ShowPropertyGrid();

#pragma endregion  HEADER InputHandler

InputHandler();

void HandleMouseButtonInput(GLFWwindow* pWindow, int button, int action, int mods);
void HandleKeyboardInput(int key, int scancode, int action, int mods);
void FrameUpdate();

void Reset();

};
