#include "InputHandler.h"

#pragma region  CLASS_SOURCE InputHandler
InputHandler* g_InputHandler;

#include "InputHandler.h"
#include "serializer.h"
#include <cstddef>
#include "ObjectManager.h"
const ClassPropertyData InputHandler::g_Properties[] = 
{
{ "ObjectID", PIDX_ObjectID, offsetof(InputHandler, m_ObjectID), 0, LXType_int, false, LXType_None, 0, 0, 0, 0,}, 
{ "Name", PIDX_Name, offsetof(InputHandler, m_Name), 0, LXType_stdstring, false, LXType_None, 0, 0, 0, 0,}, 
{ "Mouse1Pressed", PIDX_Mouse1Pressed, offsetof(InputHandler, m_Mouse1Pressed), 0, LXType_bool, false, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(InputHandler, Mouse1Pressed, bool),}, 
{ "Mouse2Pressed", PIDX_Mouse2Pressed, offsetof(InputHandler, m_Mouse2Pressed), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "CtrlHeld", PIDX_CtrlHeld, offsetof(InputHandler, m_CtrlHeld), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "Dragging", PIDX_Dragging, offsetof(InputHandler, m_Dragging), 0, LXType_bool, false, LXType_None, 0, 0, 0, 0,}, 
{ "DragDistance", PIDX_DragDistance, offsetof(InputHandler, m_DragDistance), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "LastDragDistance", PIDX_LastDragDistance, offsetof(InputHandler, m_LastDragDistance), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "DragAccumulator", PIDX_DragAccumulator, offsetof(InputHandler, m_DragAccumulator), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Adder, 0, 0, 0,}, 
{ "MouseClickPosition", PIDX_MouseClickPosition, offsetof(InputHandler, m_MouseClickPosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(InputHandler, MouseClickPosition, glm::vec2),}, 
{ "LastMouseClickPosition", PIDX_LastMouseClickPosition, offsetof(InputHandler, m_LastMouseClickPosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "MouseReleasePosition", PIDX_MouseReleasePosition, offsetof(InputHandler, m_MouseReleasePosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "MousePosition", PIDX_MousePosition, offsetof(InputHandler, m_MousePosition), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_SetCallback, 0, 0, WriteSetFunction(InputHandler, MousePosition, glm::vec2),}, 
{ "LastMousePosition", PIDX_LastMousePosition, offsetof(InputHandler, m_LastMousePosition), 0, LXType_glmvec2, false, LXType_None, 0, 0, 0, 0,}, 
{ "MouseScroll", PIDX_MouseScroll, offsetof(InputHandler, m_MouseScroll), 0, LXType_glmvec2, false, LXType_None, PropertyFlags_Adder, 0, 0, 0,}, 
};
bool InputHandler::Serialize(bool writing)
{
	bool success = g_Serializer->SerializeObject(this, writing); 
	return success;
}

#pragma endregion  CLASS_SOURCE InputHandler


InputHandler::InputHandler()
{

}


void InputHandler::HandleMouseButtonInput(GLFWwindow* pWindow, int button, int action, int mods)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_1 :
		{
			bool pressed = action == GLFW_PRESS;
			bool released = action == GLFW_RELEASE;

			SetMouse1Pressed(pressed);
			SetDragging(pressed);

			double x, y;
			glfwGetCursorPos(pWindow, &x, &y);
			glm::vec2 mousePos = glm::vec2(x, y);
			if (pressed)
			{
				SetMouseClickPosition(mousePos);
				SetDragAccumulator(glm::vec2(0, 0));
			}
			else
			{
				// released
				SetMouseReleasePosition(mousePos);

				SetLastDragDistance(m_DragAccumulator);
			}
		}
	}
}

void InputHandler::HandleKeyboardInput(int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT)
	{
		return;
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		m_CtrlHeld = (action == GLFW_PRESS);
	}
}


void InputHandler::SetMousePosition_Callback(const glm::vec2& value)
{
	m_MousePosition = value;
}

void InputHandler::SetMouseClickPosition_Callback(const glm::vec2& value)
{
	m_LastMouseClickPosition = m_MouseClickPosition;
	m_MouseClickPosition = value;
}

void InputHandler::SetMouse1Pressed_Callback(const bool& value)
{
	m_Mouse1Pressed = value;
}

void InputHandler::FrameUpdate()
{
	glm::vec2 dragDistance = glm::vec2(0, 0);
	if (m_Mouse1Pressed)
	{
		dragDistance = m_MousePosition - m_LastMousePosition;
		AddTo_DragAccumulator(dragDistance);
	}
	SetDragDistance(dragDistance);

	SetLastMousePosition(m_MousePosition);
}

void InputHandler::Reset()
{
	m_MouseScroll = glm::vec2(0,0);
}
